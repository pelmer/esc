Float_t write_tree(const char *filename = "data.root", Int_t nbevents = 10000000, Int_t compression = 0)
{
        printf("Writing %d events in a root tree with compression level %d \n",nbevents,compression);
        
        TFile *fileout = new TFile(filename,"recreate");        
        if ( fileout->IsZombie() ) {
                printf("Cannot open file \n");
                return 0.0;
        }
        fileout->SetCompressionLevel(compression);
        
        Double_t wbytes = 0.0;
        Int_t nbgamma; Int_t e[1000]; 
        for (Int_t i = 1; i < 1000; i++) e[i] = 200*i;
        
        TTree *treeout; 
        treeout = new TTree("TEST","TEST");
        treeout->Branch("mult",&nbgamma,"nbgamma/I"); treeout->Branch("e",e,"e[nbgamma]/I");
                
        // write events and compute the needed time
        TStopwatch watch; 
        watch.Start();
        for (int i = 0; i < nbevents; i++ ) {
                nbgamma = 2 + i % 5; wbytes += 4.0;  wbytes += nbgamma * 4.0;
                treeout->Fill();
        }
        watch.Stop();
        
        printf("  --> Writing rate %f MB/s [%f] \n",wbytes/(1024*1024*watch.RealTime()),wbytes/(1024*1024));
        
        fileout->Close(); delete fileout;
        
        return wbytes;
}

Float_t read_tree(const char *filename = "data.root", Double_t bytes = 10000000)
{
        printf("Reading events from a root tree \n");
        
        TFile *filein = new TFile(filename,"READ");     
        if ( filein->IsZombie() ) {
                printf("Cannot open file \n");
                return 0.0;
        }
        
        Int_t nbgamma; Int_t e[1000]; // up to 1000 energies ..should be ok 
        
        TTree *treein; 
        treein = (TTree *)filein->Get("TEST");
        if ( treein == NULL ) return 0;
        else {
                treein->SetBranchAddress("mult",&nbgamma); 
                treein->SetBranchAddress("e",&e);
        }

        // write events and compute the needed time
        TStopwatch watch; 
        watch.Start();
        treein->Draw("e","","goff");
        watch.Stop();

        cout << "  --> Reading rate " << bytes / (1024*1024*watch.RealTime()) << " MB/s"<< endl ;
        
        filein->Close(); delete filein; return bytes / (1024*1024*watch.RealTime()) ;
}

void BenchTree()
{
        
        printf("\n************ ROOT/TTree I/O  ************ \n");       
        const char *fname2 = "testio.root";
        Double_t wbytes;        
        wbytes = write_tree(fname2,100000000,0); 
        read_tree(fname2,wbytes); 
}
