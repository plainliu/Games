function getRotationPrecomputeL(precompute_L, rotationMatrix){
	let mat3 = computeSquareMatrix_3by3(rotationMatrix);
	let mat5 = computeSquareMatrix_5by5(rotationMatrix);

	let result = [precompute_L[0]];
	result = result.concat(math.multiply(mat3, precompute_L.slice(1, 4))._data);
	result = result.concat(math.multiply(mat5, precompute_L.slice(4))._data);
	return result;
}

function computeSquareMatrix_3by3(rotationMatrix){ // 计算方阵SA(-1) 3*3 
	
	// 1、pick ni - {ni}
	let n1 = [1, 0, 0, 0]; let n2 = [0, 0, 1, 0]; let n3 = [0, 1, 0, 0];

	// 2、{P(ni)} - A  A_inverse
	let pn1 = SHEval(n1[0], n1[1], n1[2], 3).slice(1, 4);
	let pn2 = SHEval(n2[0], n2[1], n2[2], 3).slice(1, 4);
	let pn3 = SHEval(n3[0], n3[1], n3[2], 3).slice(1, 4);
	let A = math.matrix([pn1, pn2, pn3]);
	A = math.transpose(A);
	let A_inverse = math.inv(A);

	// 3、用 R 旋转 ni - {R(ni)}
	let rn1 = vec4.create();
	let rn2 = vec4.create();
	let rn3 = vec4.create();
	vec4.transformMat4(rn1, n1, rotationMatrix);
	vec4.transformMat4(rn2, n2, rotationMatrix);
	vec4.transformMat4(rn3, n3, rotationMatrix);

	// 4、R(ni) SH投影 - S
	let s1 = SHEval(rn1[0], rn1[1], rn1[2], 3).slice(1, 4);
	let s2 = SHEval(rn2[0], rn2[1], rn2[2], 3).slice(1, 4);
	let s3 = SHEval(rn3[0], rn3[1], rn3[2], 3).slice(1, 4);
	let S = math.matrix([s1, s2, s3]);
	S = math.transpose(S);

	// 5、S*A_inverse
	return math.multiply(S, A_inverse);
}

function computeSquareMatrix_5by5(rotationMatrix){ // 计算方阵SA(-1) 5*5
	
	// 1、pick ni - {ni}
	let k = 1 / math.sqrt(2);
	let n1 = [1, 0, 0, 0]; let n2 = [0, 0, 1, 0]; let n3 = [k, k, 0, 0]; 
	let n4 = [k, 0, k, 0]; let n5 = [0, k, k, 0];

	// 2、{P(ni)} - A  A_inverse
	let pn1 = SHEval(n1[0], n1[1], n1[2], 3).slice(4);
	let pn2 = SHEval(n2[0], n2[1], n2[2], 3).slice(4);
	let pn3 = SHEval(n3[0], n3[1], n3[2], 3).slice(4);
	let pn4 = SHEval(n4[0], n4[1], n4[2], 3).slice(4);
	let pn5 = SHEval(n5[0], n5[1], n5[2], 3).slice(4);
	let A = math.matrix([pn1, pn2, pn3, pn4, pn5]);
	A = math.transpose(A);
	let A_inverse = math.inv(A);

	// 3、用 R 旋转 ni - {R(ni)}
	let rn1 = vec4.create();
	let rn2 = vec4.create();
	let rn3 = vec4.create();
	let rn4 = vec4.create();
	let rn5 = vec4.create();
	vec4.transformMat4(rn1, n1, rotationMatrix);
	vec4.transformMat4(rn2, n2, rotationMatrix);
	vec4.transformMat4(rn3, n3, rotationMatrix);
	vec4.transformMat4(rn4, n4, rotationMatrix);
	vec4.transformMat4(rn5, n5, rotationMatrix);

	// 4、R(ni) SH投影 - S
	let s1 = SHEval(rn1[0], rn1[1], rn1[2], 3).slice(4);
	let s2 = SHEval(rn2[0], rn2[1], rn2[2], 3).slice(4);
	let s3 = SHEval(rn3[0], rn3[1], rn3[2], 3).slice(4);
	let s4 = SHEval(rn4[0], rn4[1], rn4[2], 3).slice(4);
	let s5 = SHEval(rn5[0], rn5[1], rn5[2], 3).slice(4);
	let S = math.matrix([s1, s2, s3, s4, s5]);
	S = math.transpose(S);

	// 5、S*A_inverse
	return math.multiply(S, A_inverse);
}

function mat4Matrix2mathMatrix(rotationMatrix){

	let mathMatrix = [];
	for(let i = 0; i < 4; i++){
		let r = [];
		for(let j = 0; j < 4; j++){
			r.push(rotationMatrix[i*4+j]);
		}
		mathMatrix.push(r);
	}
	return math.matrix(mathMatrix)

}

function getMat3ValueFromRGB(precomputeL){

    let colorMat3 = [];
    for(var i = 0; i<3; i++){
        colorMat3[i] = mat3.fromValues( precomputeL[0][i], precomputeL[1][i], precomputeL[2][i],
										precomputeL[3][i], precomputeL[4][i], precomputeL[5][i],
										precomputeL[6][i], precomputeL[7][i], precomputeL[8][i] ); 
	}
    return colorMat3;
}