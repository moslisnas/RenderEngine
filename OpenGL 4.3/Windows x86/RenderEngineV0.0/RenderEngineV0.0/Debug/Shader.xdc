<?xml version="1.0"?><doc>
<members>
<member name="D:ptrdiff_t" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="2">
C++ Shader.h
Purpose: Header of Shader class

@author Alejandro Molina Quesada
@version 1.0
@date 04/03/2018

</member>
<member name="T:Shader" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="23">
<summary>
Class for shaders of the models, store information about uniform variables and attributes.
</summary>
</member>
<member name="M:Shader.#ctor" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="45">
<summary>
Constructor of <c>Shader</c> class.
</summary>
</member>
<member name="M:Shader.Dispose" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="49">
<summary>
Destructor of <c>Shader</c> class.
</summary>
</member>
<member name="M:Shader.loadCommonUniformsVertex" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="56">
<summary>
This method load more usual uniform variables for vertex shader.
</summary>
</member>
<member name="M:Shader.loadCommonUniformsGeometry" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="60">
<summary>
This method load more usual uniform variables for geometry shader.
</summary>
</member>
<member name="M:Shader.loadCommonAttributesVertex" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="64">
<summary>
This method load more usual attributes for vertex shader.
</summary>
</member>
<member name="M:Shader.loadCommonAttributesGeometry" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="68">
<summary>
This method load more usual attributes for geometry shader.
</summary>
</member>
<member name="M:Shader.loadCommonUniformsFragment" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="72">
<summary>
This method load more usual uniform variables for fragment shader.
</summary>
</member>
<member name="M:Shader.loadPhongVertexShader" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="78">
<summary>
This method load a vertex shader for classic Phong shading method.
</summary>
</member>
<member name="M:Shader.loadPhongGeometryShader" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="82">
<summary>
This method load a geometry shader for classic Phong shading method.
</summary>
</member>
<member name="M:Shader.loadPhongFragmentShader" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="86">
<summary>
This method load a fragment shader for classic Phong shading method.
</summary>
</member>
<member name="M:Shader.loadBlinnPhongVertexShader" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="90">
<summary>
This method load a vertex shader for Blinn-Phong shading method.
</summary>
</member>
<member name="M:Shader.loadBlinnPhongFragmentShader" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="94">
<summary>
This method load a fragment shader for Blinn-Phong shading method.
</summary>
</member>
<member name="M:Shader.loadPhongBumpVertexShader" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="98">
<summary>
This method load a vertex shader for a bumped Phong shading method.
</summary>
</member>
<member name="M:Shader.loadPhongBumpFragmentShader" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="102">
<summary>
This method load a fragment shader for a bumped Phong shading method.
</summary>
</member>
<member name="M:Shader.loadToonVertexShader" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="106">
<summary>
This method load a vertex shader for a toon shading method.
</summary>
</member>
<member name="M:Shader.loadToonFragmentShader" decl="true" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.h" line="110">
<summary>
This method load a fragment shader for a toon shading method.
</summary>
</member>
<member name="M:Shader.#ctor" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="4">
<summary>
Constructor of <c>Shader</c> class.
</summary>
</member>
<member name="M:Shader.Dispose" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="13">
<summary>
Destructor of <c>Shader</c> class.
</summary>
</member>
<member name="M:Shader.loadCommonUniformsVertex" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="22">
<summary>
This method load more usual uniform variables for vertex shader.
</summary>
</member>
<member name="M:Shader.loadCommonUniformsGeometry" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="36">
<summary>
This method load more usual uniform variables for geometry shader.
</summary>
</member>
<member name="M:Shader.loadCommonAttributesVertex" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="44">
<summary>
This method load more usual attributes for vertex shader.
</summary>
</member>
<member name="M:Shader.loadCommonAttributesGeometry" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="62">
<summary>
This method load more usual attributes for geometry shader.
</summary>
</member>
<member name="M:Shader.loadCommonUniformsFragment" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="70">
<summary>
This method load more usual uniform variables for fragment shader.
</summary>
</member>
<member name="M:Shader.loadPhongVertexShader" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="101">
<summary>
This method load a vertex shader for classic Phong shading method.
</summary>
</member>
<member name="M:Shader.loadPhongGeometryShader" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="111">
<summary>
This method load a geometry shader for classic Phong shading method.
</summary>
</member>
<member name="M:Shader.loadPhongFragmentShader" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="121">
<summary>
This method load a fragment shader for classic Phong shading method.
</summary>
</member>
<member name="M:Shader.loadBlinnPhongVertexShader" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="130">
<summary>
This method load a vertex shader for Blinn-Phong shading method.
</summary>
</member>
<member name="M:Shader.loadBlinnPhongFragmentShader" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="140">
<summary>
This method load a fragment shader for Blinn-Phong shading method.
</summary>
</member>
<member name="M:Shader.loadPhongBumpVertexShader" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="149">
<summary>
This method load a vertex shader for a bumped Phong shading method.
</summary>
</member>
<member name="M:Shader.loadPhongBumpFragmentShader" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="159">
<summary>
This method load a fragment shader for a bumped Phong shading method.
</summary>
</member>
<member name="M:Shader.loadToonVertexShader" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="168">
<summary>
This method load a vertex shader for a toon shading method.
</summary>
</member>
<member name="M:Shader.loadToonFragmentShader" decl="false" source="c:\users\molic\documents\github\renderengine\opengl 4.3\windows x86\renderenginev0.0\renderenginev0.0\shader.cpp" line="178">
<summary>
This method load a fragment shader for a toon shading method.
</summary>
</member>
</members>
</doc>