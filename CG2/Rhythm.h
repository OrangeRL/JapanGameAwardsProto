#pragma once
#include "Input.h"
#include "Audio.h"
#include "GameObject3D.h"

//����̎��
enum class Weapons {
	Normal,		//�ʏ�e
	Rapid,		//���˒e
	ThreeWay,	//3way�e
	Explosion,	//�����e
	Laser,		//���[�U�[�e
};

//����
enum class Judge {
	None,
	Miss,
	Good,
};

enum class Scene
{
	Title,//�^�C�g��
	Stage,//�X�e�[�W
};

struct SoundState {
	//�^�C�}�[
	float timer = 0.0f;
	//���߃J�E���g(��)
	int measureCount = 0;
	//�R���{
	int combo = 0;

	//�ꎞ��~����
	bool isPause = 0;
	//����̎��
	Weapons weapon = Weapons::Normal;
	//���ː���������
	bool isFireSucces;
	//���̒e�����Ă�܂ł̃N�[���^�C���̃t���O
	bool isFireActive = 1;
	//���[�U�[�e�̔��ˉ\�t���O
	bool isLaserActive = 0;
	//����
	Judge judge = Judge::None;

	//����̂���(�I�t�Z�b�g)
	float offset = 0.0f;
	//BGM����
	float BGMVolume = 0.5f;
	//�m�[�}��SE����
	float normalSEVolume = 0.5f;
	//�K�C�hSE����
	float guideSEVolume = 0.5f;
	//���݂̃E�F�[�u��
	float wave = 1.0f;
};

class Rhythm
{
public:

	~Rhythm();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);

	void Update(Input* input, Vector3 pos, Vector3 rot, int isDead,int stage, Scene scene, int select);

	void Draw(int isDead);

	void SoundUnload(SoundData soundData);

	void SoundPlayWave(SoundData soundData, float volume);

	void ItemSoundPlay(){ soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), itemSound, false, soundState.normalSEVolume); };
	void DamageSoundPlay(){ soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), damageSound, false, soundState.normalSEVolume); };
	void KnockSoundPlay(){ soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), knockSound, false, soundState.normalSEVolume); };
	void ScoreRisePlay(){ soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), scoreRiseSound, false, soundState.normalSEVolume); };
	void DecisionSoundPlay(){ soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(),decisionSound, false, soundState.normalSEVolume); };
	void SelectSoundPlay(){ soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(),selectSound, false, soundState.normalSEVolume); };

	void NormalShot(SoundState s, Input* input);
	void RapidShot(SoundState s, Input* input);
	void ThreeWayShot(SoundState s, Input* input);
	void ExplosionShot(SoundState s, Input* input);
	void LaserShot(SoundState s, Input* input);

	void SetWeapon(Weapons weapon) { this->soundState.weapon = weapon; }
	void SetBGMVolume(float volume) { soundState.BGMVolume = volume; }
	void SetSEVolume(float volume) { soundState.normalSEVolume = volume; }
	void SetGSEVolume(float volume) { soundState.guideSEVolume = volume; }

	void ResetRhythm();

	void ComboUp();

	SoundState GetSoundState() { return soundState; }

private:
	static const int circleNum = 2;
	GameObject3D* circle[circleNum];

	SoundState soundState;

	//�ꏬ�߂̎���
	const float maxTimer = 60.0f;
	//����̍L��
	const float judgeBreadth = 5.0f;

	//�e���˃^�C�~���O1
	float shotTiming1;
	//�e���˃^�C�~���O2
	float shotTiming2;
	//�e���˃^�C�~���O3
	float shotTiming3;
	//�e�����U�����^�C�~���O
	float reloadTiming;
	
	//�����e�̔��˂������������ǂ���(��)
	int isSuccess = 0;

	Vector3 shrinkSpeed = { 0.1f,0.1f,0.0f };

	float colorChange = 0.0f;

	float pitch = 1.0003f;

	//�T�E���h�}�l�[�W���[
	SoundManager* soundManager_ = nullptr;

	//�����ǂݍ���
	//SE
	SoundData guideSound1 = soundManager_->SoundLoadWave("Resources/rhythm2.wav");
	SoundData guideSound2 = soundManager_->SoundLoadWave("Resources/rhythm3.wav");
	SoundData guideSound3 = soundManager_->SoundLoadWave("Resources/rhythm4.wav");
	SoundData shotSound = soundManager_->SoundLoadWave("Resources/shot2.wav");
	SoundData missSound = soundManager_->SoundLoadWave("Resources/death.wav");
	SoundData laserSound = soundManager_->SoundLoadWave("Resources/shot4.wav");
	SoundData explosionSound = soundManager_->SoundLoadWave("Resources/shot3.wav");
	SoundData itemSound = soundManager_->SoundLoadWave("Resources/item.wav");
	SoundData damageSound = soundManager_->SoundLoadWave("Resources/damage.wav");
	SoundData knockSound = soundManager_->SoundLoadWave("Resources/knock.wav");
	SoundData scoreRiseSound = soundManager_->SoundLoadWave("Resources/scoreRise.wav");
	SoundData countdownSound = soundManager_->SoundLoadWave("Resources/Countdown.wav");
	SoundData decisionSound = soundManager_->SoundLoadWave("Resources/decision.wav");
	SoundData selectSound = soundManager_->SoundLoadWave("Resources/select.wav");

	//BGM
	SoundData titleBGM = soundManager_->SoundLoadWave("Resources/SHOOTRONOME.wav");
	SoundData demoBGM = soundManager_->SoundLoadWave("Resources/demo.wav");
	SoundData stage1_1BGM = soundManager_->SoundLoadWave("Resources/stage1-1.wav");
	SoundData stage1_2BGM = soundManager_->SoundLoadWave("Resources/stage1-2.wav");
	SoundData stage1_bossBGM = soundManager_->SoundLoadWave("Resources/stage1-boss.wav");

};