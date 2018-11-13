/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/11
 * @brief 工况触发
 * @details
 ******************************************************************************/

#ifndef REFINE_TRIGGER_HPP
#define REFINE_TRIGGER_HPP

#include "Message.hpp"

/*!
 * @brief 判断工况是否触发类
 */

class Trigger : public Message
{
public:
    inline void reset() {}

};


#endif //REFINE_TRIGGER_HPP
