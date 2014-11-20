#include <mpi.h>
#include <stdio.h>

double connectivity(int vIn[], int vOut[], double eProb[], char e[], int vertex, int edge)
{
	char mark[vertex]; 
	double probability = 1;
	int adjacent[vertex], av = 0;
	for (int i = 0; i < vertex; ++i)
		mark[i] = '0'; 
	for (int i = 0; i < edge; ++i)
		if (e[i] == '0') 
			probability *= (1 - eProb[i]);
		else
			probability *= eProb[i];
	av = 1;
	adjacent[0] = 0;
	mark[0] = '1';
	while (av) {
		int tav = 0;
		int tadj[vertex];
		for (int it = 0; it < av; ++it) {
			for (int i = 0; i < edge; ++i) {
				if (e[i] == '0')
					if (vIn[i] == adjacent[it]) {
						if (mark[vOut[i]] == '0') {
							tadj[tav] = vOut[i];
							mark[vOut[i]] = '1'; 
							++tav;
						}
					} else if (vOut[i] == adjacent[it]) 
						if (mark[vIn[i]] == '0') {
							tadj[tav] = vIn[i]; 
							mark[vIn[i]] = '1';
							++tav;
						}
			}
		}
		for (int i = 0; i < tav; ++i)
			adjacent[i] = tadj[i];
		av = tav;
	}

	int f = 1;
	for (int i = 0; i < vertex && f; ++i)
		if (mark[i] == '0')
			f = 0;
	return f*probability;
}
int main(int argc, char ** argv)
{
	int g[2];
	int rank, root = 0, master = 1, size;
	FILE *InFile;
	int ft;	
	InFile = fopen("InGraph","r");
	ft = fscanf (InFile, "%d", &g[0]);
	ft = fscanf (InFile, "%d", &g[1]);
	int vIn[g[1]], vOut[g[1]];
	double eProb[g[1]];

	for (int i = 0; i < g[1]; ++i) {
		ft = fscanf (InFile, "%d", &vIn[i]);
		ft = fscanf (InFile, "%d", &vOut[i]);
		ft = fscanf (InFile, "%lf", &eProb[i]);
		--vIn[i];
		--vOut[i];			
	}
	fclose (InFile);

// robit 

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);

	int eDiv=0, eMod=0;
	if (rank != master && rank != root) {
		int temp = size - 2;
		char binEdge[g[1]+1], binSize[g[1]+1];
		for (int i = g[1]; i >= 0; --i) {
			binEdge[i] = '0';
			if (temp % 2 == 1) {
				temp /= 2;
				binSize[i] = '1';
			} else {
				temp /= 2;
				binSize[i] = '0';
			}
		}
		binEdge[0] = '1';
		
		int fEnd = 0, fProf = 1;
		for (int i = 0; i < g[1]+1 && fProf; ++i)
			if (binEdge[i] == '1' && binSize[i] == '0') {
				fEnd = 0;
				fProf = 0;
			} else if (binEdge[i] == '0' && binSize[i] == '1') {
				fEnd = 1;
				fProf = 0;
			}
		fProf = 1;
		
		while (!fEnd) {
			++eDiv;
			int Perenos = 0;
			for (int i = g[1]; i >= 0; --i) { 
				if (binEdge[i] == '1' && binSize[i] == '1' && Perenos == 0)
					binEdge[i] = '0';
				else if (binEdge[i] == '1' && binSize[i] == '0' && Perenos == 0)
					binEdge[i] = '1';
				else if (binEdge[i] == '0' && binSize[i] == '0' && Perenos == 0)
					binEdge[i] = '0';
				else if (binEdge[i] == '0' && binSize[i] == '1' && Perenos == 0) {
					binEdge[i] = '1';
					Perenos = 1;
				} else if (binEdge[i] == '1' && binSize[i] == '1' && Perenos == 1)
					binEdge[i] = '1';
				else if	(binEdge[i] == '1' && binSize[i] == '0' && Perenos == 1) {
					binEdge[i] = '0';
					Perenos = 0;
				} else if (binEdge[i] == '0' && binSize[i] == '0' && Perenos == 1)
					binEdge[i] = '1';
				else if (binEdge[i] == '0' && binSize[i] == '1' && Perenos == 1)
					binEdge[i] = '0';
			}
			
			fEnd = 0;
			for (int i = 0; i < g[1]+1 && fProf; ++i)
				if (binEdge[i] == '1' && binSize[i] == '0') {
					fEnd = 0;
					fProf = 0;
				} else if (binEdge[i] == '0' && binSize[i] == '1') {
					fEnd = 1;
					fProf = 0;
				}
			fProf = 1;
		}
		

		for (int i = 0; i < g[1]+1; ++i)
			if (binEdge[i] == '1')
				eMod = (eMod << 1) + 1;
			else
				eMod = eMod << 1;
		if (rank < eMod+2)
			++eDiv;
	}

	MPI_Bcast(vIn, g[1], MPI_INT, root, MPI_COMM_WORLD); //otpravka na proceseri
	MPI_Bcast(vOut, g[1], MPI_INT, root, MPI_COMM_WORLD);
	MPI_Bcast(eProb, g[1], MPI_DOUBLE, root, MPI_COMM_WORLD); 
	MPI_Bcast(g, 2, MPI_INT, root, MPI_COMM_WORLD);
	MPI_Bcast(g, 2, MPI_INT, master, MPI_COMM_WORLD);
	for (int i = 2; i < size; ++i)
		MPI_Bcast(g, 2, MPI_INT, i, MPI_COMM_WORLD); // end
	
	double sumP = 0;
	if (rank == root) {
		int RCPU = 2;
		char eRoot[g[1]];
		for (int i = 0; i < g[1]; ++i) {
			eRoot[i] = '0';
		}

		int f = 0;
		MPI_Status status;
		MPI_Send(eRoot, g[1], MPI_CHAR, RCPU, 0, MPI_COMM_WORLD); // pervoe otprovlenie
		++RCPU;
		if (RCPU == size)
			RCPU = 2;
 
		while (!f) {

			int f1 = 0;				//formirovanie novogo sluchaya
			for (int i = g[1]-1; i >= 0 && !f1; --i)
				if (eRoot[i] == '0') {
					eRoot[i] = '1';
					f1 = 1;
				} else 
					eRoot[i] = '0'; 	//end

			f = 1;
			for (int i = 0; i < g[1]; ++i) // poslednee li otpravlenie
				if (eRoot[i] == '0')
					f = 0;

			MPI_Send(eRoot, g[1], MPI_CHAR, RCPU, 0, MPI_COMM_WORLD);
			++RCPU;
			if (RCPU == size)
				RCPU = 2;
		}

	} else if (rank == master) {	
		FILE *OutFile;	
		double buf[1], sum = 0.0;
		char eMaster[g[1]];
		OutFile = fopen("OutGraph","w");
		for (int i = 0; i < g[1]; ++i) {
			eMaster[i] = '0';
		}
		int fExit = 1;
		while (fExit) {
			
			MPI_Status status;
 			MPI_Recv(buf, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 17, MPI_COMM_WORLD, &status);
			sum += buf[0];
			
			int f1 = 0;				
			for (int i = g[1]-1; i >= 0 && !f1; --i)
				if (eMaster[i] == '0') {
					eMaster[i] = '1';
					f1 = 1;
				} else 
					eMaster[i] = '0';
			fExit = 0;
			for (int i = 0; i < g[1]; ++i)
				if (eMaster[i] == '0')
					fExit = 1;
		}
		fprintf (OutFile, "%lf", sum);
		//printf("->%lf<-\n", sum);
		fclose (OutFile);
	} else {
		//printf("->%d<-\n", eDiv);
		for (int i = 0; i < eDiv; ++i) { //eMod
			double prob[1];
			char eN[g[1]];
			MPI_Status status;
			MPI_Recv(eN, g[1], MPI_CHAR, root, 0, MPI_COMM_WORLD, &status);
			prob[0] = connectivity(vIn, vOut, eProb, eN, g[0], g[1]);	
			MPI_Send(prob, 1, MPI_DOUBLE, master, 17, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
}
