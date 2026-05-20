#pragma once
/* 
 * @file    DamageProcessor.h
 * @brief   DamageRequestを元にダメージ数を描画する。
 * @author  Yamaguchi Hayato
 * @date    2026/05/20
 * @details ダメージフォントの描画とダメージの適応を一括で行う。
 */

#include "DamageRequest.h"

namespace nsApp
{
	class DamageProcessor final
	{
	public:
		DamageProcessor() = delete;
		virtual ~DamageProcessor() = default;


	public:
		/**
		 * @brief ダメージ数を描画する。
		 * @param request ダメージフォントを描画する際に必要な構造体。
		 * @return 描画に成功できたかどうかのフラグ。
		 */
		static bool ApplyDamage(const DamageRequest& request);


	private:
		/**
		 * @brief リクエストが有効か判定する。
		 * @param request ダメージ処理に必要な情報。
		 * @return 有効な場合true。
		 */
		static bool IsValidRequest(const DamageRequest& request);

		/**
		 * @brief ダメージフォントを生成する。
		 * @param damageValue 表示するダメージ量。
		 * @param position 表示位置。
		 */
		static void SpawnDamageIndicator(int damageValue, const Vector3& position);
	};
}
