#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"
#include "Input.h"
#include "Rhythm.h"

class PlayerBullet {
public:
	//�R���X�g���N�^
	PlayerBullet();
	//�f�X�g���N�^
	~PlayerBullet();

	//������
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 playerPos, Vector3 bossPos, Weapons weapon);
	//�X�V
	void Update(Vector3 vec, float shotAngle);
	//�`��
	void Draw();

	bool IsDead() const { return isDead_; }

	int GetIsDead();
	Vector3 GetAngle();
	WorldTransform GetWorldTransform();

	void OnCollision();

	//�U��
	void AttackPress();

	void Attack(Vector3 playerPos, Vector3 bossPos, Vector3 vec, float shotAngle);
private:	//�����o�֐�
	
private:	//�����o�ϐ�
	//�e��
	const int bulletNum = 10;
	//�Q�[���I�u�W�F�N�g
	GameObject3D* gameObject;
	GameObject3D* gameObjectSub[2];
	//�\���t���O
	bool isShot;
	//�J�E���g
	int bulletCount = 0;
	//�e�̈ړ����x
	float speed = -1.0f;
	//�e�̃x�N�g��
	Vector3 velocity;
	Vector3 vec1;
	Vector3 vec2;
	//�e�̈ړ��͈�
	float canMoveArea = 120;

	//����
	static const int32_t kLifeTime = 60 * 1;
	//�f�X�^�C�}�[
	int deathTimer_ = kLifeTime;
	//�f�X�t���O
	int isDead_ = false;
	Vector3 angle = {};

	Vector3 newEnemyPos = {};
	Vector3 newPlayerPos = {};

	//�L�[����
	Input& input = Input::GetInstance();

	Weapons weapon;
};