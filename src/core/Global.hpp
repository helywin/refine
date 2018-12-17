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
        Info = 0x01,
        Warning = 0x02,
        Critical = 0x04,
        Fatal = 0x08,
        Debug = 0x10
    };
};

#endif //REFINE_GLOBAL_HPP
