// SimulationGenerator - Startup.cs (2019)
// Copyright: Ygor Rebouças Serpa


using System;
using System.Collections.Generic;
using UnityEngine;


/// <summary>
/// This script assembles everything needed to run a batch of tests
/// This script is meant to be used from the editor, to debug/develop
/// scenes and engines.
/// Also, the 'Wizard' script creates a startup object from GUI-provided
/// data to run simulations from a build in run-time.
/// </summary>
public class Startup : MonoBehaviour {
    public Wizard wizard;

    public string               OutputRoot = "";
    public PhysicsEngineType    Engine;
    public bool                 Record      = false;
    public bool                 RecordMovie = false;
    public int                  Frames = 1000;
    public SimulationType[] Simulations = {
        SimulationType.FreeFall
    };
    public ObjectsType[] ObjectTypes = {
        ObjectsType.Cubes
    };
    public int[] Ns = {
        1000
    };
//////////////
// EX JOBB ///////////////////////////////////////////////////////////////
    public  float[]      Ps                = { 0.5f                  }; //
    public  Vector3Int[] intervals         = { new Vector3Int(0,0,0) }; //
    private int          Ps_length         = 0;                         //
    private int          numberOfTestRunns = 0;                         //
    public  bool         useExjobTest      = false;                     //
    public  int          RedundancyTest    = 20;                        //
//////////////////////////////////////////////////////////////////////////	
    public SimulationParameters Template;

    private void Start() {
        Simulation.OutputRoot = this.OutputRoot;
        Runner r = this.GetComponent<Runner>();
        List<SimulationParameters> list = new List<SimulationParameters>();
        if (useExjobTest) 
		{
            list.AddRange(AutomatedTest());
        }
        else
        {
			foreach (int n in this.Ns)                            {
                foreach (SimulationType sim in this.Simulations)  {
                    foreach (ObjectsType type in this.ObjectTypes){
                        SimulationParameters instance = this.Template.ShallowCopy();
                        instance.Engine               = this.Engine;
                        instance.Frames               = this.Frames;
                        instance.RecordSimulations    = this.Record;
                        instance.RecordMovie          = this.RecordMovie;
                        instance.Simulation           = sim;
                        instance.ObjectsType          = type;
                        instance.N                    = n;

                        /*static*/
                        instance.Ps            = this.Ps[0];
                        instance.OutRecordPath = "";
                        list.Add(instance);
                    }
                }
            }

        }

        r.wizard     = this.wizard;
        r.parameters =  list.ToArray();
        r.enabled    = true;
    }



 //////////////
 // EX JOBB ///////////////////////////////////////////////////////////////
    private List<SimulationParameters> AutomatedTest()
    {

        string procentStaticPreityName  = "_procent";


        int redundancy          = RedundancyTest;

        Ps_length = this.Ps.Length;

        int runPerTest = Ps_length *
                         redundancy *
                         this.Simulations.Length *
                         this.ObjectTypes.Length;


        foreach (Vector3Int interval in this.intervals)
        {
            int interval_count = ( interval[1]- interval[0]) / interval[2];
            interval_count += 1; //den första omgången
            interval_count *= runPerTest;
            numberOfTestRunns += interval_count;
        }


      




       // numberOfTestRunns *= interval_count;

        int currentRun = 0;


        List<SimulationParameters> list = new List<SimulationParameters>();



        for(int ps = 0; ps < Ps_length; ps++)
		{
            string rootDir1 = (int)(Ps[ps]*100.0f) +  procentStaticPreityName + "/";
            foreach (SimulationType sim in this.Simulations)
            {
                string rootDir2 = System.IO.Path.Combine(rootDir1, sim.ToString() + "/");
                foreach (ObjectsType type in this.ObjectTypes)
                {
                    string objektPrettyName = type.ToString() + "/";
                    foreach (Vector3Int interval in this.intervals)
                    {
                        list.AddRange(IntervalTest(interval[0],
                                                   interval[1],
                                                   interval[2],
                                                   redundancy,
                                                   rootDir2,
                                                   objektPrettyName,
                                                   sim,
                                                   type,
                                                   ref currentRun,
                                                   ps));
                    }


                }


            }
        }

       
       return list;

    }

    private List<SimulationParameters> IntervalTest(int            start, 
                                                    int            end, 
                                                    int            intervall, 
                                                    int            redundancy,
                                                    string         rootDir2,
                                                    string         objektPathName,
                                                    SimulationType sim,
                                                    ObjectsType    type,
                                                    ref int        currentRun,
                                                    int            curent_ps)
    {
        List<SimulationParameters> intervallRun = new List<SimulationParameters>();

        string intervalName = start + "_" + end + "_" + intervall;

        string rootDir3 = System.IO.Path.Combine(rootDir2, intervalName + "/");

        int numberOfIntervalTest = (end - start) / intervall;
        numberOfIntervalTest += 1;


        for (int i = start; i <= end; i += intervall)
        {
         
            string rootDir4 = System.IO.Path.Combine(rootDir3, "N"+i + "/" + objektPathName);

            int currentIntervallTest = i / intervall;
            //reduntan check
            for (int j = 1; j <= redundancy; ++j)
            {
                ++currentRun;
                string rootDir5 = System.IO.Path.Combine(rootDir4, "T"+j + "/");

                SimulationParameters instance = this.Template.ShallowCopy();
                instance.Engine               = this.Engine;
                instance.Frames               = this.Frames;
                instance.RecordSimulations    = this.Record;
                instance.RecordMovie          = this.RecordMovie;
                instance.Simulation           = sim;
                instance.ObjectsType          = type;
                instance.N                    = i;

                /*static*/
                instance.Ps            = this.Ps[curent_ps];
                instance.OutRecordPath = rootDir5;



                //info about test progresion
                instance.simsRun = new Vector2Int(currentRun, this.numberOfTestRunns);
                instance.statRun = new Vector2Int(curent_ps, Ps_length);
                instance.inteRun = new Vector2Int(currentIntervallTest, numberOfIntervalTest);
                instance.reduRun = new Vector2Int(j, redundancy);

                instance.Interval = new Vector3Int(start, end, intervall);

                intervallRun.Add(instance);
            }
        }
        return intervallRun;
    }


/////////////////////////////////////////////////////////////////////////////////////////////////////////
}