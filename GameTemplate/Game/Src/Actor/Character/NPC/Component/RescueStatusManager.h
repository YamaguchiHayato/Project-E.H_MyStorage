#pragma once

#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	class RescueStatusManager
	{
    public:
        /**
         * @brief ダウン回数を加算する処理。
         */
        inline void AddDownCount()
        {
            m_downCount++;
        }

        /**
         * @brief ダウン回数をリセットする処理。
		 * @param helperWeapon 救助に使用する武器の種類。
		 * @return 救助に必要な時間（フレーム数）。武器の種類やダウン回数に応じて変動する。
         */
        int CalculateRequiredHelpTime(WeaponType helperWeapon) const;

        /**
		 * @brief 救助状態をリセットする処理。
         *        これにより、NPCは再び救助される可能性がある状態になります。
         */
        inline void ResetRescueStatus()
        {
            m_isBeingHelped = false;
        }


	/* セッター。*/ 
    public:
        /**
         * @brief ダウン回数をリセットする処理。
		 * @param flag リセットするかどうかを示すフラグ。
         *        trueの場合はダウン回数を0にリセットし、falseの場合はリセットしない。
         */
        inline void SetBeingHelped(bool flag)
        {
            m_isBeingHelped = flag; 
        }


    /* ゲッター。*/
    public:
        /**
		 * @brief ダウン回数を取得する処理。
		 * @return ダウン回数。
         */
        inline bool IsBeingHelped() const
        { 
            return m_isBeingHelped; 
        }


    private:
        int m_downCount = 0;          // ダウン回数
        bool m_isBeingHelped = false; // 救助予約中か
    };
}
