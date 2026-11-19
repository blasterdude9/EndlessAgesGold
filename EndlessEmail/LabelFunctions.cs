using System;
using System.Threading;
using System.Reflection;
using Word;
namespace EndlessAdmin
{
	/// <summary>
	/// Summary description for LabelFunctions.
	/// </summary>
	public class LabelFunctions
	{
		public LabelFunctions()
		{
			//
			// TODO: Add constructor logic here
			//
		}

		public void PrintLabelArray(int count,string [] addressArray,string templateName, bool bufferVerticle,bool bufferHoriz)
		{
		
			int globalTracker = 0;
			object saveChanges = false;
			object originalFormat = Missing.Value;
			object routeDocument = Missing.Value;
			//This is comparable to CoCreateInstance 
		
			//get stencil count
			Application appSize = new Application();  
			appSize.Visible=false;//false for backgroud modifier
			object template=templateName;//;Missing.Value;
			object newTemplate=Missing.Value;
			object documentType=Missing.Value;
			object visible=true;
			object optionalTwo = Missing.Value;
			object defaultThis = Missing.Value;
			Document docSizeTest = appSize.Documents.Add(ref template, ref newTemplate, ref documentType, ref visible);
			int labelsPerPage = docSizeTest.Words.Count;
			int rowCount = docSizeTest.Tables.Item(1).Rows.Count;
			int collumnsCount = docSizeTest.Tables.Item(1).Columns.Count;
			int writableCollumnCount = 0;
			int writableRowCount = 0;
			if(bufferHoriz)
			{
				bool switchOn = true;
				for(int j=0;j<collumnsCount;j++)
				{
					if(switchOn)
					{
						writableCollumnCount++;
						switchOn = false;
					}
					else
						switchOn = true;

				}
			}
			if(bufferVerticle)
			{
				bool switchOn = true;
				for(int j=0;j<rowCount;j++)
				{
					if(switchOn)
					{
						writableRowCount++;
						switchOn = false;
					}
					else
						switchOn = true;

				}
			}
			else
				writableRowCount = rowCount;
		   
			appSize.Quit(ref saveChanges, ref originalFormat, ref routeDocument);

			int cycleCount = (count/(writableRowCount*writableCollumnCount)) + 1;
		
		
		 
			for(int cycle = 0;cycle<cycleCount;cycle++)
			{//pages
				Application app = new Application();  
				app.Visible=false;//false for backgroud modifier
				Document doc = app.Documents.Add(ref template, ref newTemplate, ref documentType, ref visible);
				int tracer = labelsPerPage-2;
				int write = 0;
				bool rowOn = true;
				int colCounter = 0;
				for(int row=0;row<doc.Words.Count;row++)
				{

					if(tracer > -1)
					{
						if(write == 0)
						{
							if(rowOn == true || bufferVerticle == false)
								if(globalTracker < count)
								{
									doc.Words.Item(tracer).Text = addressArray[globalTracker];
									globalTracker++;
								}
							tracer--;
							write = 1;
						}
						else
						{
							write = 0;
							tracer--;
						}
					}

					if(colCounter < collumnsCount) 
						colCounter++;
					else
					{
						if(rowOn)
							rowOn = false;
						else
							rowOn = true;
						colCounter = 0;
					}

				}

				object optional = Missing.Value;
		  #if OFFICEXP
		  doc.PrintOut2000(
		  #else
				doc.PrintOut(
		  #endif
					ref optional,ref optional,ref optional,ref optional,
					ref optional,ref optional,ref optional,ref optional,ref optional,ref optional,
					ref optional,ref optional,ref optional,ref optional,ref optional,ref optional,ref optional,ref optional);
				Thread.Sleep (5000);
				app.Quit(ref saveChanges, ref originalFormat, ref routeDocument);
			}//end pages
		}
	}
}
