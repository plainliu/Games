class PRTMaterial extends Material {

    constructor(vertexShader, fragmentShader) {
        // let prtlight = getMat3ValueFromRGB(precomputeL[guiParams.envmapId])

        super({
            // 'uPrecomputeLR': { type: 'matrix3fv', value: prtlight[0] },
            // 'uPrecomputeLG': { type: 'matrix3fv', value: prtlight[1] },
            // 'uPrecomputeLB': { type: 'matrix3fv', value: prtlight[2] },
            'uPrecomputeLR': { type: 'updatedInRealTime', value: null },
            'uPrecomputeLG': { type: 'updatedInRealTime', value: null },
            'uPrecomputeLB': { type: 'updatedInRealTime', value: null },
        }, [
            'aPrecomputeLT'
        ], vertexShader, fragmentShader, null);
    }
}

async function buildPRTMaterial(vertexPath, fragmentPath) {
    let vertexShader = await getShaderString(vertexPath);
    let fragmentShader = await getShaderString(fragmentPath);

    return new PRTMaterial(vertexShader, fragmentShader);
}