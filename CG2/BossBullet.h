#pragma once
#include "GameObject3D.h"
#include "Boss.h"

class BossBullet
{
public:
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 player, Vector3 enemy, BossPhase phase);
	void Update(BossPhase phase,Vector3 player);
	void Draw();

	WorldTransform GetWorldTransform();
	//��������ꏊ��ݒ�
	Vector3 SetTransform(Vector3 transform);
	Vector3 SetScale(Vector3 scale);

	bool IsDead()const { return isDelete_; }

	void Aim(Vector3 player, Vector3 enemy);
	int Random(int minValue, int maxValue);
private:
	GameObject3D* gameObject = nullptr;

	//������܂ł̎���
	// 60 * ������܂ł̎���:
	static const int32_t deleteTime = 60 * 150;
	Vector3 speed;
	//�^�C�}�[
	int32_t deleteTimer_ = deleteTime;
	//�t���O
	bool isDelete_ = false;

	Vector3 posA;
	Vector3 posB;
	Vector3 posC;
	int selectPos;
	bool isSelect = false;
	Vector3 moveSpeed = { 0,0,0 };//���ʒu
	Vector3 speed = { 0,0.1f,0 };//�����ʒu

};

