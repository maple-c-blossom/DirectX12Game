#include "Sound.h"


using namespace MCB;

void MCB::SoundManager::ReleasexAudio2()
{
	xAudio2.Reset();
}

void MCB::SoundManager::DeleteSound(unsigned int SoundHandle)
{
	delete[] sounds[SoundHandle].pBuffer;

	sounds[SoundHandle].pBuffer = 0;
	sounds[SoundHandle].bufferSize = 0;
	sounds[SoundHandle].wfex = {};
	sounds[SoundHandle].free = true;
}

void MCB::SoundManager::AllDeleteSound()
{
	for (int i = 0; i < MaxSound; i++)
	{
		if (sounds[i].pBuffer != nullptr)
		{
			delete[] sounds[i].pBuffer;

			sounds[i].pBuffer = 0;
			sounds[i].bufferSize = 0;
			sounds[i].wfex = {};
			sounds[i].free = true;
		}
	}
}

SoundManager::SoundManager()
{
	HRESULT result = S_FALSE;

	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	assert(SUCCEEDED(result) && "XAudioインスタンス生成時エラー");

	result = xAudio2->CreateMasteringVoice(&masterVoice);

}

MCB::SoundManager::~SoundManager()
{
	xAudio2.Reset();
	for (int i = 0; i < MaxSound; i++)
	{
		if (sounds[i].pBuffer != nullptr)
		{
			delete[] sounds[i].pBuffer;

			sounds[i].pBuffer = 0;
			sounds[i].bufferSize = 0;
			sounds[i].wfex = {};
		}
	}

}

unsigned int MCB::SoundManager::LoadWaveSound(const char* fileName)
{
	HRESULT result;
	unsigned int handleNum = 0;

	for (int i = 0; i < MaxSound; i++)
	{
		if (sounds[i].free)continue;
		if (sounds[i].name == fileName) return i;
	}

	for (int i = 0; i < MaxSound; i++)
	{
		if (sounds[i].free)
		{
			handleNum = i;
			sounds[i].free = false;
		}
	}
	

	std::ifstream file;
	file.open(fileName, std::ios_base::binary);

	assert(file.is_open() && "SoundLoadError");

	SoundManager::RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	SoundManager::FormatChunk format{};
	file.read((char*)&format, sizeof(format.chunk));

	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	SoundManager::ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0)
	{
		assert(0);
	}

	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	file.close();

	sounds[handleNum].wfex = format.fmt;
	sounds[handleNum].pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	sounds[handleNum].bufferSize = data.size;
	sounds[handleNum].name = fileName;

	return handleNum;

}

void MCB::SoundManager::PlaySoundWave(unsigned int soundHandle,bool isLoop)
{
	HRESULT result = S_FALSE;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result =xAudio2.Get()->CreateSourceVoice(&pSourceVoice, &sounds[soundHandle].wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = sounds[soundHandle].pBuffer;
	buf.AudioBytes = sounds[soundHandle].bufferSize;
	if (isLoop) buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	else buf.LoopCount = 0;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();

}

