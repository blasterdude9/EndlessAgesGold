D3DMATRIX Exporter::GetMatrixFromMaxStruct(Matrix3* m)
{
	D3DMATRIX matrixCreation;

    GL_matrixFunctions.InitMat(matrixCreation.m);
	Point3 row;
	row = m->GetRow(0);
	matrixCreation._11 = row.x;
	matrixCreation._12 = row.z;
	matrixCreation._13 = row.y;
	row = m->GetRow(1);
	matrixCreation._21 = row.x;
	matrixCreation._22 = row.z;
	matrixCreation._23 = row.y;
	row = m->GetRow(2);
	matrixCreation._31 = row.x;
	matrixCreation._32 = row.z;
	matrixCreation._33 = row.y;
	row = m->GetRow(3);
	matrixCreation._41 = row.x;
	matrixCreation._42 = row.z;
	matrixCreation._43 = row.y;
		
    return matrixCreation;
}

//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
BOOL Exporter::nodeEnum(INode* node, int indentLevel,int tracer) 
{
	TSTR indent = GetIndent(indentLevel);
	if (node->IsGroupHead()) {
		indentLevel++;
	}
		ObjectState os = node->EvalWorldState(m_skeletalExport->m_firstFrame*GetTicksPerFrame()); 
		// The obj member of ObjectState is the actual object we will export.
		if (os.obj) {
			//for(int loop=0;loop<(frameCount+1);loop++)
			BOOL physiqueExists = FALSE;
            ExportNodeTM(node,0,&physiqueExists);
			/*TSTR buf;
			node->GetUserPropBuffer(buf);
			AfxMessageBox(buf);*/
			
			LPSTR nodeName = FixupName(node->GetName());
			
			Matrix3 tm = node->GetObjTMAfterWSM(0);
			
			//LogVertexAssignments(node,2);
		}
    GL_index++;
	// For each child of this node, we recurse into ourselves 
	// until no more children are found.
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!nodeEnum(node->GetChildNode(c), indentLevel,tracer))
			return FALSE;
	}
	return TRUE;
}