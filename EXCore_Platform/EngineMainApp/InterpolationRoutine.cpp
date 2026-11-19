void EngineXWin::Interpolate(D3DRMVERTEX *StageVec, 
						D3DRMVERTEX *StartVec, 
						D3DRMVERTEX *EndVec,
                        int Siz, 
					    int NumOfStages, 
					    int ThisStage)
{
 int    cntr;
 float propfactor;

 StageVec   = StartVec;
 if (NumOfStages <= 0)
  NumOfStages = 1;
 propfactor = (float)ThisStage/NumOfStages;
 for (cntr = 0; cntr < Siz; cntr++)
 {
  StageVec[cntr].position.x += propfactor * (EndVec[cntr].position.x - StartVec[cntr].position.x);
  StageVec[cntr].position.y += propfactor * (EndVec[cntr].position.y - StartVec[cntr].position.y);
  StageVec[cntr].position.z += propfactor * (EndVec[cntr].position.z - StartVec[cntr].position.z);

  StageVec[cntr].normal.x += propfactor * (EndVec[cntr].normal.x
                           - StartVec[cntr].normal.x);
  StageVec[cntr].normal.y += propfactor * (EndVec[cntr].normal.y
                           - StartVec[cntr].normal.y);
  StageVec[cntr].normal.z += propfactor * (EndVec[cntr].normal.z
                           - StartVec[cntr].normal.z);
 }
}