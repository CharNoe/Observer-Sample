#pragma once

#define EVENT_DEF(StructName)                                                            \
    struct StructName;                                                                   \
    virtual void ReceiveEvent(const StructName& param)                                   \
    {                                                                                    \
    }                                                                                    \
    struct StructName
