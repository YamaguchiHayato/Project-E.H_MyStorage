#include "stdafx.h"
#include "RescueStatusManager.h"
#include <unordered_map>

namespace
{
    const int BASE_HELP_TIME = 90;          // 基本救助時間（1.5秒）
    const int PENALTY_TIME_PER_DOWN = 30;   // 1ダウンごとのペナルティ（0.5秒）

    // 武器ごとの救助速度倍率
    const std::unordered_map<nsApp::WeaponType, float> JOB_SPEED_MAP = {
        { nsApp::WeaponType::Wand, 0.5f }, // 杖は2倍速
        { nsApp::WeaponType::None, 1.0f }
    };
}

namespace nsApp
{
    int RescueStatusManager::CalculateRequiredHelpTime(WeaponType helperWeapon) const
    {
        // 基本時間 + ダウン回数によるペナルティ
        int requiredTime = BASE_HELP_TIME + (m_downCount * PENALTY_TIME_PER_DOWN);

        // ジョブによる倍率を適用
        float multiplier = JOB_SPEED_MAP.count(helperWeapon) ? JOB_SPEED_MAP.at(helperWeapon) : 1.0f;

        return static_cast<int>(requiredTime * multiplier);
    }
}