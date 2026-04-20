/*
** Sample Framework for deko3d Applications
**   CShader.cpp: Utility class for loading shaders from the filesystem
*/
#include "CShader.h"

struct DkshHeader
{
    uint32_t magic; // DKSH_MAGIC
    uint32_t header_sz; // sizeof(DkshHeader)
    uint32_t control_sz;
    uint32_t code_sz;
    uint32_t programs_off;
    uint32_t num_programs;
};

bool CShader::load(CMemPool& pool, const char* path)
{
    FILE* f;
    DkshHeader hdr;
    void* ctrlmem;

    m_codemem.destroy();

    f = fopen(path, "rb");
    if (!f) return false;

    if (!fread(&hdr, sizeof(hdr), 1, f))
        goto _fail0;

    ctrlmem = malloc(hdr.control_sz);
    if (!ctrlmem)
        goto _fail0;

    rewind(f);
    if (!fread(ctrlmem, hdr.control_sz, 1, f))
        goto _fail1;

    m_codemem = pool.allocate(hdr.code_sz, DK_SHADER_CODE_ALIGNMENT);
    if (!m_codemem)
        goto _fail1;

    if (!fread(m_codemem.getCpuAddr(), hdr.code_sz, 1, f))
        goto _fail2;

    dk::ShaderMaker{m_codemem.getMemBlock(), m_codemem.getOffset()}
        .setControl(ctrlmem)
        .setProgramId(0)
        .initialize(m_shader);

    free(ctrlmem);
    fclose(f);
    return true;

_fail2:
    m_codemem.destroy();
_fail1:
    free(ctrlmem);
_fail0:
    fclose(f);
    return false;
}

bool CShader::loadFromMemory(CMemPool& pool, const uint8_t* data, size_t size)
{
    const DkshHeader* hdr;
    void* ctrlmem;

    if (!data || size < sizeof(DkshHeader))
        return false;

    m_codemem.destroy();

    hdr = reinterpret_cast<const DkshHeader*>(data);

    ctrlmem = malloc(hdr->control_sz);
    if (!ctrlmem)
        return false;

    /* Copy control data */
    memcpy(ctrlmem, reinterpret_cast<const uint8_t*>(data), hdr->control_sz);

    m_codemem = pool.allocate(hdr->code_sz, DK_SHADER_CODE_ALIGNMENT);
    if (!m_codemem)
        goto _fail;

    /* Copy code data */
    memcpy(m_codemem.getCpuAddr(), reinterpret_cast<const uint8_t*>(data) + hdr->control_sz, hdr->code_sz);

    dk::ShaderMaker{m_codemem.getMemBlock(), m_codemem.getOffset()}
        .setControl(ctrlmem)
        .setProgramId(0)
        .initialize(m_shader);

    free(ctrlmem);
    return true;

_fail:
    m_codemem.destroy();
    free(ctrlmem);
    return false;
}
