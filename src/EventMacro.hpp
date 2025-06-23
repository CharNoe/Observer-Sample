#pragma once

#define EVENT_DEF(StructName)                                                            \
public:                                                                                  \
    struct StructName;                                                                   \
                                                                                         \
private:                                                                                 \
    virtual void ReceiveEvent(const StructName& param)                                   \
    {                                                                                    \
    }                                                                                    \
                                                                                         \
public:                                                                                  \
    struct StructName
