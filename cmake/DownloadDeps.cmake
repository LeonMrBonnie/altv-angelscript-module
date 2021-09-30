# Deps download

set(__deps_version "v5")

function(download_deps)
    if(EXISTS "${ANGELSCRIPT_LIB_PATH}")
        return()
    endif()
    file(DOWNLOAD "https://github.com/LeonMrBonnie/angelscript/releases/download/${__deps_version}/${ANGELSCRIPT_LIB}" "${ANGELSCRIPT_LIB_PATH}")
endfunction()
