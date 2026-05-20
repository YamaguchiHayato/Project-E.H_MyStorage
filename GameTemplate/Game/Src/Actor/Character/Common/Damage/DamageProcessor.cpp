#include "stdafx.h"
#include "DamageProcessor.h"
#include "PresentDamageIndicator.h"

namespace nsApp
{
	bool DamageProcessor::ApplyDamage(const DamageRequest& request)
	{
		if (!IsValidRequest(request))
			return false;

		/* ダメージを付与。*/
		request.target->ApplyDamage(request.damageAmount);

		/* フォントを生成。*/
		SpawnDamageIndicator(request.damageAmount, request.hitPosition);

		return true;
	}


	bool DamageProcessor::IsValidRequest(const DamageRequest& request)
	{
		/* 目標が存在しない場合、処理を行わない。*/
		if (request.target == nullptr)
			return false;

		/* ダメージ数が0の場合、処理を行わない。*/
		if (request.damageAmount <= 0)
			return false;

		return true;
	}


	void DamageProcessor::SpawnDamageIndicator(int damageValue, const Vector3& position)
	{
		auto damageIndicator = NewGO<PresentDamageIndicator>(0, "damageUI");
		damageIndicator->Init(damageValue, position);
	}
}