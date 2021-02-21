#pragma once

#include "angelscript/include/angelscript.h"
#include <iostream>
#include <fstream>

namespace Helpers
{
    class BytecodeStream : public asIBinaryStream
    {
    public:
        BytecodeStream(std::string fileName, bool read = false) : readOnly(read), file(fileName,  (readOnly ? std::ios::in : std::ios::out) | std::ios::binary) 
        {
            // If we write to the file, first clear the file content
            if(!readOnly)
            {
                file.clear();
            }
        }
        ~BytecodeStream()
        {
            Close();
        }
        
        int Write(const void* ptr, asUINT size)
        {
            if(size == 0) return 0;
            file.write((char*)ptr, size);
            return 0;
        }
        int Read(void* ptr, asUINT size)
        {
            if(size == 0) return 0;
            file.read((char*)ptr, size);
            return 0;
        }

        void Close()
        {
            file.close();
        }
    
    protected:
        std::fstream file;
        bool readOnly;
    };
}
