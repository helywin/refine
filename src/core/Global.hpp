//
// Created by jiang.wenqiang on 2018/12/17.
//

#ifndef REFINE_GLOBAL_HPP
#define REFINE_GLOBAL_HPP

namespace Re {
    enum RunningStatus
    {
        Stop,
        Running,
        Pause
    };

    enum RunningCommand
    {
        NoCommand,
        CommandStop,
        CommandResume,
        CommandPause
    };

    enum MessageType
    {
        NoMessageType = 0x00,
        Info = 0x01,
        Warning = 0x02,
        Critical = 0x04,
        Fatal = 0x08,
        Debug = 0x10,
        InfoTypes = (Info | Warning | Critical | Fatal | Debug),
        CanSend = 0x20,
        CanReceive = 0x40,
        Can = (CanSend | CanReceive),
        Popout = 0x1000     //把messagebox也加入进来
    };
    Q_DECLARE_FLAGS(MessageTypes, MessageType)

    Q_DECLARE_OPERATORS_FOR_FLAGS(MessageTypes)

    enum RevolveFlag
    {
        NoWork = 0x00,
        Communicate = 0x01,
        Collect = 0x02,
        TransformData = 0x04,
        RecordFrame = 0x08,
        TimingStop = 0x10
    };
    Q_DECLARE_FLAGS(RevolveFlags, RevolveFlag)
    Q_DECLARE_OPERATORS_FOR_FLAGS(RevolveFlags)

    enum LineStipple
    {
        SolidLine,
        DotLine,
        DashDotLine,
        DashLine
    };
};

#endif //REFINE_GLOBAL_HPP
