//
// Created by jiang.wenqiang on 2018/7/12.
//

#ifndef CORE_MODES_H
#define CORE_MODES_H

#include "Criteria.h"

class ModeAcc : public Criteria {
public:
    enum class Subtype : int {
        CylinderDeactivation = 1,
        CylinderReactivation = 2,
        Elasticity = 3,
        FullLoad = 4,
        FullLoadHundredKph = 5,
        FullLoadThroughGears = 6,
        LowEndTorque = 7,
        PartLoadConstantPedal = 8,
        PartLoadFallingPedal = 9,
        PartLoadRisingPedal = 10
    };
    enum class Criteria : int {
        NinetyPercentTorqueRange = 1,
        NinetyPercentTorqueThreshold = 2,
        AccelerationGradient = 3,
        AccelerationSteps = 4,
        Correlation = 5,
        Elasticity = 6,
        EngineFreeAcceleration = 7,
        EngineSpeedChange = 8,
        ExpectedAcceleration = 9,
        FullLoadHundredKph = 10,
        FullLoadOverHundredKph = 11,
        FullLoadThroughGears = 12,
        LowEndTorque = 13,
        MaximumExpectedTorque = 14,
        Noise = 15,
        PedalMap = 16,
        ReferenceAcceleration = 17,
        RpmLimiter = 18,
        Steadiness = 19,
        Surge = 20,
        TorqueBuildUp = 21,
        TorqueResponse = 22,
        TorqueSmoothness = 23,
        Vibrations = 24,
        WideTorqueRange = 25
    };

    //functions

    Fail init(QFile &file) override;


};

class ModeDvw : public Criteria {
public:
    enum class Subtype : int {
        Creep = 1,
        HillClimbing = 2,
        Lunch = 3,
        Normal = 4,
        RollingStop = 5,
        VehicleStop = 6
    };
    enum class Criteria : int {
        AccelerationPeak = 1,
        AccelerationPerformance = 2,
        BreakReleaseBump = 3,
        BreakReleaseSurge = 4,
        Bump = 5,
        ClutchSurge = 6,
        EngagementSteadiness = 7,
        EngineSpeedFluctuation = 8,
        EngineSpeedOvershoot = 9,
        EngineSpeedUndershoot = 10,
        HillClimbing = 11,
        InitialBump = 12,
        Jerks = 13,
        Overshoot = 14,
        ResponseDelay = 15,
        Shock = 16,
        Stumble = 17
    };
};

class ModeGrs : public Criteria {
public:
    enum class Subtype : int {
        CoastBreakOnDownshift = 1,
        CoastBreakOnUpshift = 2,
        KickDownTipInDownshift = 3,
        ManeuveringEngage = 4,
        PowerOnDownshift = 5,
        PowerOnUpshift = 6,
        SelectorLeverChange = 7,
        ShiftAbort = 8,
        TipOutUpshift = 9
    };
    enum class Criteria : int {
        AxGradientsAfterRatioChange = 1,
        AxGradientsBeforeRatioChange = 2,
        AxGradientsDuringRatioChange = 3,
        AxIncreaseDelay = 4,
        Bump = 5,
        DecelerationCharacteristic = 6,
        Delay = 7,
        EngineSpeedAtEngagement = 8,
        EngineSpeedDecrease = 9,
        EngineSpeedDrift = 10,
        EngineSpeedFluctuations = 11,
        EngineSpeedIncrease = 12,
        EngineSpeedOvershoot = 13,
        EngineSpeedUndershoot = 14,
        InitialBump = 15,
        Jerks = 16,
        JerksAtManeuveringEngage = 17,
        Kick = 18,
        Noise = 19,
        RatioChangeSteadiness = 20,
        ShiftCharacteristic = 21,
        ShiftDelay = 22,
        ShiftDelayAtSelectorLeverChange = 23,
        ShiftDuration = 24,
        ShiftEndSteadiness = 25,
        ShiftStartSteadiness = 26,
        Shock = 27,
        StabilizationDuration = 28,
        TractionReduction = 29,
        Vibrations = 30
    };
};

class ModeTpn : public Criteria {
public:
    enum class Subtype : int {
        AfterClosedPedal = 1,
        AfterConstantSpeed = 2,
        DuringAcceleration = 3,
        ShortTipInAfterClosedPedal = 4,
        ShortTipInAfterConstantSpeed = 5,
        ShortTipInDuringAcceleration = 6
    };
    enum class Criteria : int {
        AbsoluteTorque = 1,
        Flare = 2,
        InitialBump = 3,
        Jerks = 4,
        Kick = 5,
        Noise = 6,
        ResponseDelay = 7,
        Stumble = 8,
        TorqueBuildUp = 9,
        TorqueSmoothness = 10,
        Vibration = 11
    };
};

class ModeTpt : public Criteria {
public:
    enum class Subtype : int {
        AfterAcceleration = 1,
        AfterConstantSpeed = 2,
        DuringDeceleration = 3
    };
    enum class Criteria : int {
        InitialBump = 1,
        Jerks = 2,
        Kick = 3,
        ResponseDelay = 4,
        Vibration = 5
    };
};

class ModeCrs : public Criteria {
public:
    enum class Subtype : int {
        CancelHard = 1,
        CancelSoft = 2,
        ConstantSpeedAfterOverride = 3,
        EndOfOverride = 4,
        EndOfResumeDown = 5,
        EndOfResumeUp = 6,
        EndOfTapDown = 7,
        EndOfTapUp = 8,
        ResumeDown = 9,
        ResumeUp = 10,
        SetDuringAcceleration = 11,
        SetDuringCcAcceleration = 12,
        SetDuringConstantSpeed = 13,
        SetDuringMotoring = 14,
        TapDown = 15,
        TapUp = 16,
    };
    enum class Criteria : int {
        AxExpected = 1,
        AxGradient = 2,
        AxOvershoot = 3,
        AxSteps = 4,
        AxSurge = 5,
        AxUndershoot = 6,
        ResponseDelay = 7,
    };
};

#endif //CORE_MODES_H
