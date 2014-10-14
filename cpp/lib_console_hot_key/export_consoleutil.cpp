/**
 * @file export_consoleutil.cpp
 * @brief 导出consoleutil为python模块
 * @author zsl
 * @version 1.0
 * @date 2012-10-25
 */

// Copyright (C) 2012 Feitian Technologies Co., Ltd. All rights reserved.

#include "consoleutil.h"
#include "PyCallback.h"

#include <boost/python.hpp>

namespace bpy = boost::python;


bpy::object CmdReader_readline(CmdReader &reader)
{
    int ret = reader.readline();
    if (!ret)
        return bpy::str(reader.getline() );
    else
        return bpy::long_(ret);
}


BOOST_PYTHON_MODULE(consoleutil)
{
    bpy::def("adjust_cursor", AdjustCursor, bpy::args("delta"));
    bpy::def("set_cursor_insert", SetCursorInsert, (bpy::arg("insertmode") = false) );
    bpy::def("clear_input", ClearInput, bpy::args("maxlen", "cursor_pos") );

    bpy::enum_<KEY_FLAG>("KEY_FLAG")
        .value("FN_FLAG", FN_FLAG)
        .value("EXT_FLAG", EXT_FLAG)
        .value("CTRL_FLAG", CTRL_FLAG)
        .value("ALT_FLAG", ALT_FLAG)
        .value("SHIFT_FLAG", SHIFT_FLAG)
        ;
    bpy::class_<CmdReader> CmdReader_Wrapper("CmdReader");
    CmdReader_Wrapper
        .def("readline", CmdReader_readline)
        .def("setline", &CmdReader::setline)
        .def("getline", &CmdReader::getline, bpy::return_value_policy<bpy::reference_existing_object>())
        .def("register_key", &CmdReader::registerKey, bpy::args("key") )
        .def("unregister_key", &CmdReader::unregisterKey)
        .def("unregister_all", &CmdReader::unregisterAll)
        .def("isregistered", &CmdReader::isRegistered)
        .add_property("line", CmdReader_Wrapper.attr("getline"), CmdReader_Wrapper.attr("setline") )
        .add_property("cursor_pos", &CmdReader::getCursorPos, &CmdReader::setCursorPos)
        ;
}
