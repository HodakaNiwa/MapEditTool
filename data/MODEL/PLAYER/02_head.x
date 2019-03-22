xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 193;
 -1.99223;0.17553;1.44744;,
 -0.76096;0.17553;2.34200;,
 -1.44744;1.30744;4.45476;,
 -3.78944;1.30744;2.75319;,
 0.76096;0.17553;2.34200;,
 1.44744;1.30744;4.45476;,
 1.99223;0.17553;1.44744;,
 3.78944;1.30744;2.75319;,
 2.46253;0.17553;0.00000;,
 4.68401;1.30744;0.00000;,
 1.99223;0.17553;-1.44744;,
 3.78944;1.30744;-2.75319;,
 1.99223;0.17553;-1.44744;,
 0.76096;0.17553;-2.34200;,
 1.44744;1.30744;-4.45476;,
 3.78944;1.30744;-2.75319;,
 -0.76096;0.17553;-2.34200;,
 -1.44744;1.30744;-4.45476;,
 -1.99223;0.17553;-1.44744;,
 -3.78944;1.30744;-2.75319;,
 -1.99223;0.17553;-1.44744;,
 -2.46253;0.17553;0.00000;,
 -4.68401;1.30744;0.00000;,
 -3.78944;1.30744;-2.75319;,
 -1.99223;3.07040;6.13145;,
 -5.21572;3.07040;3.78944;,
 1.99223;3.07040;6.13145;,
 5.21572;3.07040;3.78944;,
 6.44698;3.07040;0.00000;,
 5.21572;3.07040;-3.78944;,
 1.99223;3.07040;-6.13145;,
 5.21572;3.07040;-3.78944;,
 -1.99223;3.07040;-6.13145;,
 -5.21572;3.07040;-3.78944;,
 -6.44698;3.07040;0.00000;,
 -5.21572;3.07040;-3.78944;,
 -2.34200;5.29188;7.20795;,
 -6.13145;5.29188;4.45476;,
 2.34200;5.29188;7.20795;,
 6.13145;5.29188;4.45476;,
 7.57888;5.29188;0.00000;,
 6.13145;5.29188;-4.45476;,
 2.34200;5.29188;-7.20795;,
 6.13145;5.29188;-4.45476;,
 -2.34200;5.29188;-7.20795;,
 -6.13145;5.29188;-4.45476;,
 -7.57888;5.29188;0.00000;,
 -6.13145;5.29188;-4.45476;,
 -2.46253;7.75441;7.57888;,
 -6.44698;7.75441;4.68401;,
 2.46253;7.75441;7.57888;,
 6.44698;7.75441;4.68401;,
 7.96891;7.75441;0.00000;,
 6.44698;7.75441;-4.68401;,
 2.46253;7.75441;-7.57888;,
 6.44698;7.75441;-4.68401;,
 -2.46253;7.75441;-7.57888;,
 -6.44698;7.75441;-4.68401;,
 -7.96891;7.75441;0.00000;,
 -6.44698;7.75441;-4.68401;,
 -2.34200;10.21694;7.20795;,
 -6.13145;10.21694;4.45476;,
 2.34200;10.21694;7.20795;,
 6.13145;10.21694;4.45476;,
 7.57888;10.21694;0.00000;,
 6.13145;10.21694;-4.45476;,
 2.34200;10.21694;-7.20795;,
 6.13145;10.21694;-4.45476;,
 -2.34200;10.21694;-7.20795;,
 -6.13145;10.21694;-4.45476;,
 -7.57888;10.21694;0.00000;,
 -6.13145;10.21694;-4.45476;,
 -1.99223;12.43842;6.13145;,
 -5.21572;12.43842;3.78944;,
 1.99223;12.43842;6.13145;,
 5.21572;12.43842;3.78944;,
 6.44698;12.43842;0.00000;,
 5.21572;12.43842;-3.78944;,
 1.99223;12.43842;-6.13145;,
 5.21572;12.43842;-3.78944;,
 -1.99223;12.43842;-6.13145;,
 -5.21572;12.43842;-3.78944;,
 -6.44698;12.43842;0.00000;,
 -5.21572;12.43842;-3.78944;,
 -1.44744;14.20139;4.45476;,
 -3.78944;14.20139;2.75319;,
 1.44744;14.20139;4.45476;,
 3.78944;14.20139;2.75319;,
 4.68401;14.20139;0.00000;,
 3.78944;14.20139;-2.75319;,
 1.44744;14.20139;-4.45476;,
 3.78944;14.20139;-2.75319;,
 -1.44744;14.20139;-4.45476;,
 -3.78944;14.20139;-2.75319;,
 -4.68401;14.20139;0.00000;,
 -3.78944;14.20139;-2.75319;,
 -0.76096;15.33328;2.34200;,
 -1.99223;15.33328;1.44744;,
 0.76096;15.33328;2.34200;,
 1.99223;15.33328;1.44744;,
 2.46253;15.33328;0.00000;,
 1.99223;15.33328;-1.44744;,
 0.76096;15.33328;-2.34200;,
 1.99223;15.33328;-1.44744;,
 -0.76096;15.33328;-2.34200;,
 -1.99223;15.33328;-1.44744;,
 -2.46253;15.33328;0.00000;,
 -1.99223;15.33328;-1.44744;,
 0.00000;-0.21449;-0.00000;,
 0.76096;0.17553;-2.34200;,
 -0.76096;0.17553;-2.34200;,
 0.00000;15.72332;0.00000;,
 0.76096;15.33328;-2.34200;,
 -0.76096;15.33328;-2.34200;,
 -0.54683;-1.31173;0.54683;,
 0.00000;-1.31173;0.77333;,
 0.00000;-0.87368;1.42892;,
 -1.01040;-0.87368;1.01040;,
 0.54683;-1.31173;0.54683;,
 1.01040;-0.87368;1.01040;,
 0.77333;-1.31173;-0.00000;,
 1.42892;-0.87368;-0.00000;,
 0.54683;-1.31173;-0.54683;,
 1.01040;-0.87368;-1.01040;,
 0.00000;-1.31173;-0.77333;,
 0.00000;-0.87368;-1.42892;,
 -0.54683;-1.31173;-0.54683;,
 -1.01040;-0.87368;-1.01040;,
 -0.77333;-1.31173;0.00000;,
 -1.42892;-0.87368;0.00000;,
 -0.54683;-1.31173;0.54683;,
 -1.01040;-0.87368;1.01040;,
 0.00000;-0.21808;1.86698;,
 -1.32015;-0.21808;1.32015;,
 1.32015;-0.21808;1.32015;,
 1.86698;-0.21808;-0.00000;,
 1.32015;-0.21808;-1.32015;,
 0.00000;-0.21808;-1.86698;,
 -1.32015;-0.21808;-1.32015;,
 -1.86698;-0.21808;0.00000;,
 -1.32015;-0.21808;1.32015;,
 0.00000;0.55524;2.02080;,
 -1.42892;0.55524;1.42892;,
 1.42892;0.55524;1.42892;,
 2.02080;0.55524;0.00000;,
 1.42892;0.55524;-1.42892;,
 0.00000;0.55524;-2.02080;,
 -1.42892;0.55524;-1.42892;,
 -2.02080;0.55524;0.00000;,
 -1.42892;0.55524;1.42892;,
 0.00000;1.32857;1.86698;,
 -1.32015;1.32857;1.32015;,
 1.32015;1.32857;1.32015;,
 1.86698;1.32857;0.00000;,
 1.32015;1.32857;-1.32015;,
 0.00000;1.32857;-1.86698;,
 -1.32015;1.32857;-1.32015;,
 -1.86698;1.32857;0.00000;,
 -1.32015;1.32857;1.32015;,
 0.00000;1.98417;1.42892;,
 -1.01040;1.98417;1.01040;,
 1.01040;1.98417;1.01040;,
 1.42892;1.98417;0.00000;,
 1.01040;1.98417;-1.01040;,
 0.00000;1.98417;-1.42892;,
 -1.01040;1.98417;-1.01040;,
 -1.42892;1.98417;0.00000;,
 -1.01040;1.98417;1.01040;,
 0.00000;2.42222;0.77333;,
 -0.54683;2.42222;0.54683;,
 0.54683;2.42222;0.54683;,
 0.77333;2.42222;0.00000;,
 0.54683;2.42222;-0.54683;,
 0.00000;2.42222;-0.77333;,
 -0.54683;2.42222;-0.54683;,
 -0.77333;2.42222;0.00000;,
 -0.54683;2.42222;0.54683;,
 0.00000;-1.46557;-0.00000;,
 0.00000;-1.46557;-0.00000;,
 0.00000;-1.46557;-0.00000;,
 0.00000;-1.46557;-0.00000;,
 0.00000;-1.46557;-0.00000;,
 0.00000;-1.46557;-0.00000;,
 0.00000;-1.46557;-0.00000;,
 0.00000;-1.46557;-0.00000;,
 0.00000;2.57603;0.00000;,
 0.00000;2.57603;0.00000;,
 0.00000;2.57603;0.00000;,
 0.00000;2.57603;0.00000;,
 0.00000;2.57603;0.00000;,
 0.00000;2.57603;0.00000;,
 0.00000;2.57603;0.00000;,
 0.00000;2.57603;0.00000;;
 
 164;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;12,13,14,15;,
 4;13,16,17,14;,
 4;16,18,19,17;,
 4;20,21,22,23;,
 4;21,0,3,22;,
 4;3,2,24,25;,
 4;2,5,26,24;,
 4;5,7,27,26;,
 4;7,9,28,27;,
 4;9,11,29,28;,
 4;15,14,30,31;,
 4;14,17,32,30;,
 4;17,19,33,32;,
 4;23,22,34,35;,
 4;22,3,25,34;,
 4;25,24,36,37;,
 4;24,26,38,36;,
 4;26,27,39,38;,
 4;27,28,40,39;,
 4;28,29,41,40;,
 4;31,30,42,43;,
 4;30,32,44,42;,
 4;32,33,45,44;,
 4;35,34,46,47;,
 4;34,25,37,46;,
 4;37,36,48,49;,
 4;36,38,50,48;,
 4;38,39,51,50;,
 4;39,40,52,51;,
 4;40,41,53,52;,
 4;43,42,54,55;,
 4;42,44,56,54;,
 4;44,45,57,56;,
 4;47,46,58,59;,
 4;46,37,49,58;,
 4;49,48,60,61;,
 4;48,50,62,60;,
 4;50,51,63,62;,
 4;51,52,64,63;,
 4;52,53,65,64;,
 4;55,54,66,67;,
 4;54,56,68,66;,
 4;56,57,69,68;,
 4;59,58,70,71;,
 4;58,49,61,70;,
 4;61,60,72,73;,
 4;60,62,74,72;,
 4;62,63,75,74;,
 4;63,64,76,75;,
 4;64,65,77,76;,
 4;67,66,78,79;,
 4;66,68,80,78;,
 4;68,69,81,80;,
 4;71,70,82,83;,
 4;70,61,73,82;,
 4;73,72,84,85;,
 4;72,74,86,84;,
 4;74,75,87,86;,
 4;75,76,88,87;,
 4;76,77,89,88;,
 4;79,78,90,91;,
 4;78,80,92,90;,
 4;80,81,93,92;,
 4;83,82,94,95;,
 4;82,73,85,94;,
 4;85,84,96,97;,
 4;84,86,98,96;,
 4;86,87,99,98;,
 4;87,88,100,99;,
 4;88,89,101,100;,
 4;91,90,102,103;,
 4;90,92,104,102;,
 4;92,93,105,104;,
 4;95,94,106,107;,
 4;94,85,97,106;,
 3;1,0,108;,
 3;4,1,108;,
 3;6,4,108;,
 3;8,6,108;,
 3;10,8,108;,
 3;109,10,108;,
 3;110,109,108;,
 3;20,110,108;,
 3;21,20,108;,
 3;0,21,108;,
 3;97,96,111;,
 3;96,98,111;,
 3;98,99,111;,
 3;99,100,111;,
 3;100,101,111;,
 3;101,112,111;,
 3;112,113,111;,
 3;113,107,111;,
 3;107,106,111;,
 3;106,97,111;,
 4;114,115,116,117;,
 4;115,118,119,116;,
 4;118,120,121,119;,
 4;120,122,123,121;,
 4;122,124,125,123;,
 4;124,126,127,125;,
 4;126,128,129,127;,
 4;128,130,131,129;,
 4;117,116,132,133;,
 4;116,119,134,132;,
 4;119,121,135,134;,
 4;121,123,136,135;,
 4;123,125,137,136;,
 4;125,127,138,137;,
 4;127,129,139,138;,
 4;129,131,140,139;,
 4;133,132,141,142;,
 4;132,134,143,141;,
 4;134,135,144,143;,
 4;135,136,145,144;,
 4;136,137,146,145;,
 4;137,138,147,146;,
 4;138,139,148,147;,
 4;139,140,149,148;,
 4;142,141,150,151;,
 4;141,143,152,150;,
 4;143,144,153,152;,
 4;144,145,154,153;,
 4;145,146,155,154;,
 4;146,147,156,155;,
 4;147,148,157,156;,
 4;148,149,158,157;,
 4;151,150,159,160;,
 4;150,152,161,159;,
 4;152,153,162,161;,
 4;153,154,163,162;,
 4;154,155,164,163;,
 4;155,156,165,164;,
 4;156,157,166,165;,
 4;157,158,167,166;,
 4;160,159,168,169;,
 4;159,161,170,168;,
 4;161,162,171,170;,
 4;162,163,172,171;,
 4;163,164,173,172;,
 4;164,165,174,173;,
 4;165,166,175,174;,
 4;166,167,176,175;,
 3;115,114,177;,
 3;118,115,178;,
 3;120,118,179;,
 3;122,120,180;,
 3;124,122,181;,
 3;126,124,182;,
 3;128,126,183;,
 3;130,128,184;,
 3;169,168,185;,
 3;168,170,186;,
 3;170,171,187;,
 3;171,172,188;,
 3;172,173,189;,
 3;173,174,190;,
 3;174,175,191;,
 3;175,176,192;;
 
 MeshMaterialList {
  2;
  164;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\game127\\Desktop\\�Q�[���f��\\2�N��\\2018�N�A�E��i�p\\���f��\\obj\\player\\face.jpg";
   }
  }
  Material {
   0.008000;0.008000;0.008000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  150;
  -0.249423;-0.951288;0.181217;,
  -0.095271;-0.951288;0.293214;,
  0.095271;-0.951288;0.293214;,
  0.249423;-0.951288;0.181217;,
  0.308304;-0.951288;-0.000000;,
  0.249423;-0.951288;-0.181217;,
  0.095271;-0.951288;-0.293214;,
  -0.095271;-0.951288;-0.293214;,
  -0.249423;-0.951288;-0.181217;,
  -0.308304;-0.951288;0.000000;,
  -0.474752;-0.809714;0.344927;,
  -0.181339;-0.809714;0.558103;,
  0.181339;-0.809714;0.558103;,
  0.474752;-0.809714;0.344927;,
  0.586825;-0.809713;-0.000000;,
  0.474752;-0.809714;-0.344927;,
  0.181339;-0.809714;-0.558103;,
  -0.181339;-0.809714;-0.558103;,
  -0.474752;-0.809714;-0.344927;,
  -0.586825;-0.809713;0.000000;,
  -0.653961;-0.588717;0.475129;,
  -0.249791;-0.588717;0.768776;,
  0.249791;-0.588717;0.768776;,
  0.653961;-0.588717;0.475129;,
  0.808339;-0.588717;-0.000000;,
  0.653961;-0.588717;-0.475129;,
  0.249791;-0.588717;-0.768776;,
  -0.249791;-0.588717;-0.768776;,
  -0.653961;-0.588717;-0.475129;,
  -0.808339;-0.588717;0.000000;,
  -0.769247;-0.309677;0.558891;,
  -0.293826;-0.309676;0.904305;,
  0.293826;-0.309676;0.904305;,
  0.769247;-0.309677;0.558891;,
  0.950842;-0.309677;-0.000000;,
  0.769247;-0.309677;-0.558891;,
  0.293826;-0.309676;-0.904305;,
  -0.293826;-0.309676;-0.904305;,
  -0.769247;-0.309677;-0.558891;,
  -0.950842;-0.309677;0.000000;,
  -0.809017;-0.000000;0.587785;,
  -0.309017;-0.000000;0.951057;,
  0.309017;-0.000000;0.951057;,
  0.809017;-0.000000;0.587785;,
  1.000000;-0.000000;-0.000000;,
  0.809017;-0.000000;-0.587785;,
  0.309017;-0.000000;-0.951057;,
  -0.309017;-0.000000;-0.951057;,
  -0.809017;-0.000000;-0.587785;,
  -1.000000;-0.000000;0.000000;,
  -0.769247;0.309677;0.558890;,
  -0.293826;0.309677;0.904305;,
  0.293826;0.309677;0.904304;,
  0.769247;0.309677;0.558890;,
  0.950842;0.309678;0.000000;,
  0.769247;0.309677;-0.558891;,
  0.293826;0.309677;-0.904305;,
  -0.293826;0.309677;-0.904304;,
  -0.769247;0.309677;-0.558890;,
  -0.950842;0.309678;0.000000;,
  -0.653961;0.588716;0.475130;,
  -0.249791;0.588716;0.768777;,
  0.249791;0.588716;0.768777;,
  0.653961;0.588716;0.475130;,
  0.808340;0.588716;0.000000;,
  0.653961;0.588716;-0.475130;,
  0.249791;0.588716;-0.768777;,
  -0.249791;0.588716;-0.768777;,
  -0.653961;0.588716;-0.475130;,
  -0.808340;0.588716;0.000000;,
  -0.474752;0.809714;0.344927;,
  -0.181339;0.809714;0.558104;,
  0.181339;0.809714;0.558104;,
  0.474752;0.809714;0.344927;,
  0.586825;0.809713;0.000000;,
  0.474752;0.809714;-0.344927;,
  0.181339;0.809714;-0.558104;,
  -0.181339;0.809714;-0.558104;,
  -0.474752;0.809714;-0.344927;,
  -0.586825;0.809713;0.000000;,
  -0.249423;0.951288;0.181217;,
  -0.095271;0.951288;0.293215;,
  0.095271;0.951288;0.293215;,
  0.249423;0.951288;0.181217;,
  0.308305;0.951288;0.000000;,
  0.249423;0.951288;-0.181217;,
  0.095271;0.951288;-0.293215;,
  -0.095271;0.951288;-0.293215;,
  -0.249423;0.951288;-0.181217;,
  -0.308305;0.951288;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.269136;-0.924733;0.269136;,
  0.000000;-0.924733;0.380617;,
  0.269136;-0.924733;0.269136;,
  0.380617;-0.924733;-0.000000;,
  0.269136;-0.924733;-0.269136;,
  0.000000;-0.924733;-0.380617;,
  -0.269136;-0.924733;-0.269136;,
  -0.380617;-0.924733;0.000000;,
  -0.498494;-0.709230;0.498494;,
  0.000000;-0.709230;0.704977;,
  0.498494;-0.709230;0.498494;,
  0.704977;-0.709230;-0.000000;,
  0.498494;-0.709230;-0.498494;,
  0.000000;-0.709230;-0.704977;,
  -0.498494;-0.709230;-0.498494;,
  -0.704977;-0.709230;0.000000;,
  -0.652733;-0.384549;0.652733;,
  0.000000;-0.384550;0.923104;,
  0.652733;-0.384549;0.652733;,
  0.923104;-0.384550;-0.000000;,
  0.652733;-0.384549;-0.652733;,
  0.000000;-0.384550;-0.923104;,
  -0.652733;-0.384549;-0.652733;,
  -0.923104;-0.384550;0.000000;,
  -0.707107;0.000000;0.707107;,
  0.000000;0.000000;1.000000;,
  0.707107;0.000000;0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;-0.707107;,
  0.000000;0.000000;-1.000000;,
  -0.707107;0.000000;-0.707107;,
  -1.000000;0.000000;0.000000;,
  -0.652734;0.384549;0.652734;,
  0.000000;0.384549;0.923105;,
  0.652734;0.384549;0.652734;,
  0.923105;0.384549;0.000000;,
  0.652734;0.384549;-0.652734;,
  0.000000;0.384549;-0.923105;,
  -0.652734;0.384549;-0.652734;,
  -0.923105;0.384549;0.000000;,
  -0.498494;0.709230;0.498494;,
  0.000000;0.709230;0.704977;,
  0.498494;0.709230;0.498494;,
  0.704977;0.709230;0.000000;,
  0.498494;0.709230;-0.498494;,
  0.000000;0.709230;-0.704977;,
  -0.498494;0.709230;-0.498494;,
  -0.704977;0.709230;0.000000;,
  -0.269128;0.924738;0.269128;,
  0.000000;0.924737;0.380606;,
  0.269128;0.924738;0.269128;,
  0.380606;0.924737;0.000000;,
  0.269128;0.924738;-0.269128;,
  0.000000;0.924737;-0.380606;,
  -0.269128;0.924738;-0.269128;,
  -0.380606;0.924737;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;;
  164;
  4;0,1,11,10;,
  4;1,2,12,11;,
  4;2,3,13,12;,
  4;3,4,14,13;,
  4;4,5,15,14;,
  4;5,6,16,15;,
  4;6,7,17,16;,
  4;7,8,18,17;,
  4;8,9,19,18;,
  4;9,0,10,19;,
  4;10,11,21,20;,
  4;11,12,22,21;,
  4;12,13,23,22;,
  4;13,14,24,23;,
  4;14,15,25,24;,
  4;15,16,26,25;,
  4;16,17,27,26;,
  4;17,18,28,27;,
  4;18,19,29,28;,
  4;19,10,20,29;,
  4;20,21,31,30;,
  4;21,22,32,31;,
  4;22,23,33,32;,
  4;23,24,34,33;,
  4;24,25,35,34;,
  4;25,26,36,35;,
  4;26,27,37,36;,
  4;27,28,38,37;,
  4;28,29,39,38;,
  4;29,20,30,39;,
  4;30,31,41,40;,
  4;31,32,42,41;,
  4;32,33,43,42;,
  4;33,34,44,43;,
  4;34,35,45,44;,
  4;35,36,46,45;,
  4;36,37,47,46;,
  4;37,38,48,47;,
  4;38,39,49,48;,
  4;39,30,40,49;,
  4;40,41,51,50;,
  4;41,42,52,51;,
  4;42,43,53,52;,
  4;43,44,54,53;,
  4;44,45,55,54;,
  4;45,46,56,55;,
  4;46,47,57,56;,
  4;47,48,58,57;,
  4;48,49,59,58;,
  4;49,40,50,59;,
  4;50,51,61,60;,
  4;51,52,62,61;,
  4;52,53,63,62;,
  4;53,54,64,63;,
  4;54,55,65,64;,
  4;55,56,66,65;,
  4;56,57,67,66;,
  4;57,58,68,67;,
  4;58,59,69,68;,
  4;59,50,60,69;,
  4;60,61,71,70;,
  4;61,62,72,71;,
  4;62,63,73,72;,
  4;63,64,74,73;,
  4;64,65,75,74;,
  4;65,66,76,75;,
  4;66,67,77,76;,
  4;67,68,78,77;,
  4;68,69,79,78;,
  4;69,60,70,79;,
  4;70,71,81,80;,
  4;71,72,82,81;,
  4;72,73,83,82;,
  4;73,74,84,83;,
  4;74,75,85,84;,
  4;75,76,86,85;,
  4;76,77,87,86;,
  4;77,78,88,87;,
  4;78,79,89,88;,
  4;79,70,80,89;,
  3;1,0,90;,
  3;2,1,90;,
  3;3,2,90;,
  3;4,3,90;,
  3;5,4,90;,
  3;6,5,90;,
  3;7,6,90;,
  3;8,7,90;,
  3;9,8,90;,
  3;0,9,90;,
  3;80,81,91;,
  3;81,82,91;,
  3;82,83,91;,
  3;83,84,91;,
  3;84,85,91;,
  3;85,86,91;,
  3;86,87,91;,
  3;87,88,91;,
  3;88,89,91;,
  3;89,80,91;,
  4;92,93,101,100;,
  4;93,94,102,101;,
  4;94,95,103,102;,
  4;95,96,104,103;,
  4;96,97,105,104;,
  4;97,98,106,105;,
  4;98,99,107,106;,
  4;99,92,100,107;,
  4;100,101,109,108;,
  4;101,102,110,109;,
  4;102,103,111,110;,
  4;103,104,112,111;,
  4;104,105,113,112;,
  4;105,106,114,113;,
  4;106,107,115,114;,
  4;107,100,108,115;,
  4;108,109,117,116;,
  4;109,110,118,117;,
  4;110,111,119,118;,
  4;111,112,120,119;,
  4;112,113,121,120;,
  4;113,114,122,121;,
  4;114,115,123,122;,
  4;115,108,116,123;,
  4;116,117,125,124;,
  4;117,118,126,125;,
  4;118,119,127,126;,
  4;119,120,128,127;,
  4;120,121,129,128;,
  4;121,122,130,129;,
  4;122,123,131,130;,
  4;123,116,124,131;,
  4;124,125,133,132;,
  4;125,126,134,133;,
  4;126,127,135,134;,
  4;127,128,136,135;,
  4;128,129,137,136;,
  4;129,130,138,137;,
  4;130,131,139,138;,
  4;131,124,132,139;,
  4;132,133,141,140;,
  4;133,134,142,141;,
  4;134,135,143,142;,
  4;135,136,144,143;,
  4;136,137,145,144;,
  4;137,138,146,145;,
  4;138,139,147,146;,
  4;139,132,140,147;,
  3;93,92,148;,
  3;94,93,148;,
  3;95,94,148;,
  3;96,95,148;,
  3;97,96,148;,
  3;98,97,148;,
  3;99,98,148;,
  3;92,99,148;,
  3;140,141,149;,
  3;141,142,149;,
  3;142,143,149;,
  3;143,144,149;,
  3;144,145,149;,
  3;145,146,149;,
  3;146,147,149;,
  3;147,140,149;;
 }
 MeshTextureCoords {
  193;
  0.548903;0.924042;,
  0.521143;0.924042;,
  0.536621;0.898522;,
  0.589423;0.898522;,
  0.486830;0.924042;,
  0.471353;0.898522;,
  0.459070;0.924042;,
  0.418551;0.898522;,
  0.448467;0.924042;,
  0.398382;0.898522;,
  0.459070;0.924042;,
  0.418551;0.898522;,
  0.435381;0.473600;,
  0.480246;0.473600;,
  0.455232;0.440328;,
  0.369894;0.440328;,
  0.535702;0.473600;,
  0.560715;0.440328;,
  0.580566;0.473600;,
  0.646053;0.440328;,
  0.548903;0.924042;,
  0.559507;0.924042;,
  0.609592;0.898522;,
  0.589423;0.898522;,
  0.548903;0.858774;,
  0.621580;0.858774;,
  0.459070;0.858774;,
  0.386394;0.858774;,
  0.358634;0.858774;,
  0.386394;0.858774;,
  0.435381;0.388505;,
  0.317924;0.388505;,
  0.580566;0.388505;,
  0.698024;0.388505;,
  0.649340;0.858774;,
  0.621580;0.858774;,
  0.556789;0.808689;,
  0.642226;0.808689;,
  0.451184;0.808689;,
  0.365748;0.808689;,
  0.333115;0.808689;,
  0.365748;0.808689;,
  0.422636;0.323204;,
  0.284557;0.323204;,
  0.593311;0.323204;,
  0.731391;0.323204;,
  0.674859;0.808689;,
  0.642226;0.808689;,
  0.559507;0.753169;,
  0.649340;0.753169;,
  0.448467;0.753169;,
  0.358634;0.753169;,
  0.324321;0.753169;,
  0.358634;0.753169;,
  0.418244;0.250818;,
  0.273059;0.250818;,
  0.597703;0.250818;,
  0.742889;0.250818;,
  0.683653;0.753169;,
  0.649340;0.753169;,
  0.556789;0.697650;,
  0.642226;0.697650;,
  0.451184;0.697650;,
  0.365748;0.697650;,
  0.333115;0.697650;,
  0.365748;0.697650;,
  0.422636;0.178431;,
  0.284557;0.178431;,
  0.593311;0.178431;,
  0.731391;0.178431;,
  0.674859;0.697650;,
  0.642226;0.697650;,
  0.548903;0.647565;,
  0.621580;0.647565;,
  0.459070;0.647565;,
  0.386394;0.647565;,
  0.358634;0.647565;,
  0.386394;0.647565;,
  0.435381;0.113130;,
  0.317924;0.113130;,
  0.580566;0.113130;,
  0.698024;0.113130;,
  0.649340;0.647565;,
  0.621580;0.647565;,
  0.536621;0.607817;,
  0.589423;0.607817;,
  0.471353;0.607817;,
  0.418551;0.607817;,
  0.398382;0.607817;,
  0.418551;0.607817;,
  0.455232;0.061307;,
  0.369894;0.061307;,
  0.560715;0.061307;,
  0.646053;0.061307;,
  0.609592;0.607817;,
  0.589423;0.607817;,
  0.521143;0.582297;,
  0.548903;0.582297;,
  0.486830;0.582297;,
  0.459070;0.582297;,
  0.448467;0.582297;,
  0.459070;0.582297;,
  0.480246;0.028035;,
  0.435381;0.028035;,
  0.535702;0.028035;,
  0.580566;0.028035;,
  0.559507;0.582297;,
  0.548903;0.582297;,
  0.503987;0.932835;,
  0.486830;0.924042;,
  0.521143;0.924042;,
  0.503987;0.573504;,
  0.486830;0.582297;,
  0.521143;0.582297;,
  0.000000;0.875000;,
  0.125000;0.875000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.875000;,
  0.250000;0.750000;,
  0.375000;0.875000;,
  0.375000;0.750000;,
  0.500000;0.875000;,
  0.500000;0.750000;,
  0.625000;0.875000;,
  0.625000;0.750000;,
  0.750000;0.875000;,
  0.750000;0.750000;,
  0.875000;0.875000;,
  0.875000;0.750000;,
  1.000000;0.875000;,
  1.000000;0.750000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.250000;0.125000;,
  0.375000;0.125000;,
  0.500000;0.125000;,
  0.625000;0.125000;,
  0.750000;0.125000;,
  0.875000;0.125000;,
  1.000000;0.125000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;;
 }
}