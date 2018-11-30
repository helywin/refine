/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/11
 * @brief 软件信息
 * @details 软件信息存储
 ******************************************************************************/

#ifndef REFINE_VERSION_HPP
#define REFINE_VERSION_HPP

#include <QtCore/QString>
#include <QtCore/QDate>

/*!
 * @brief 记录软件详细信息的类
 * 后续加上其他版本信息，比如特性
 */


class Version
{
public:
    //! \brief 版本标识
    enum Identifier
    {
        None = 0x00,    //!< 无
        Demo = 0x01,    //!< 测试
        Alpha = 0x02,   //!< Alpha版
        Beta = 0x04,    //!< Beta版
        Gamma = 0x08,   //!< 接近正式版
        Release = 0x10  //!< 发行版
    };

private:
    const static char _major = 0;   //! \brief 主要版本号
    const static char _micro = 0;   //! \brief 次要版本号
    const static char _minor = 16;   //! \brief 最小版本号
    const static char _build = 1;   //! \brief 构建版本
    const static char _year = 18;   //! \brief 构建时间，年
    const static char _month = 11;  //! \brief 构建时间，月
    const static char _day = 30;    //! \brief 构建时间，日
    const static Identifier _identifier = Identifier::Alpha; //! \brief 版本标识

public:
    static QString str();
    /*!
     * @brief 返回主要版本号
     * @return 主要版本号
     */
    inline static char major() { return _major; }
    /*!
     * @brief 返回次要版本号
     * @return 次要版本号
     */
    inline static char micro() { return _micro; }
    /*!
     * @brief 返回最小版本号
     * @return 最小版本号
     */
    inline static char minor() { return _minor; }
    /*!
     * @brief 返回构建版本号
     * @return 构建版本号
     */
    inline static char build() { return _build; }
    /*!
     * @brief 返回构建时间年
     * @return 构建时间年
     */
    inline static char year() { return _year; }
    /*!
     * @brief 返回构建时间月
     * @return 构建时间月
     */
    inline static char month() { return _month; }
    /*!
     * @brief 返回构建时间日
     * @return 构建时间日
     */
    inline static char day() { return _day; }
    /*!
     * @brief 返回构建时间
     * @return 构建时间
     */
    inline static QDate date() { return QDate(_year + 2000, _month, _day); }
    /*!
     * @brief 返回版本标识
     * @return 版本标识
     */
    inline static char identifier() { return _identifier; }
};

#endif //REFINE_VERSION_HPP
