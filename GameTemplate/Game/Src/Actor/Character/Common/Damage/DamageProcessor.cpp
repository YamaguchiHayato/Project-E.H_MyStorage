#include "stdafx.h"
#include "DamageProcessor.h"
#include "PresentDamageIndicator.h"

namespace nsApp
{
	bool DamageProcessor::ApplyDamage(const DamageRequest& request)
	{
#ifdef _DEBUG
		char log[256];
		sprintf_s(
			log,
			"[DamageProcessor] target=%p damage=%d pos=(%.1f, %.1f, %.1f)\n",
			request.target,
			request.damageAmount,
			request.hitPosition.x,
			request.hitPosition.y,
			request.hitPosition.z
		);
		OutputDebugStringA(log);
#endif

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