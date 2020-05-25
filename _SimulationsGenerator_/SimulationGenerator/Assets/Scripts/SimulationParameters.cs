// SimulationGenerator - SimulationParameters.cs (2019)
// Copyright: Ygor Rebouças Serpa


using System;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;


[Serializable]
public class SimulationParameters {
//////////////
// EX JOBB ////////////////////////////////////////////////////////////////////////////////
    [Header("testStatusInfo")]
    public string IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII;
    public Vector2Int simsRun = new Vector2Int(0, 0);
    public Vector2Int statRun = new Vector2Int(0, 0);
    public Vector2Int inteRun = new Vector2Int(0, 0);
    public Vector2Int reduRun = new Vector2Int(0, 0);
    public string IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII;
    public Vector3Int Interval = new Vector3Int(0,0,0);
    public string IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII;
//////////////////////////////////////////////////////////////////////////////////////////
    [Header("General Parameters")]
    [HideInInspector] public PhysicsEngineType Engine = PhysicsEngineType.PhysX;
    public SimulationType Simulation = SimulationType.FreeFall;
    public int   N                    = 1000; // number of objects
 //////////////
// EX JOBB ///////////////////////////////////////////////////////////////  
    public float Ps                   = 0.0f;  // procent static objekts
//////////////////////////////////////////////////////////////////////////
    public int   Frames               = 1000; // number of frames
    public bool  RecordSimulations   = false; // if the simulations are to be recorded to disk
    public bool  RecordMovie         = false; // records per-frame screenshots
//////////////
// EX JOBB ///////////////////////////////////////////////////////////////
    public string OutRecordPath; // For the specilasied ExWork automated testing
//////////////////////////////////////////////////////////////////////////								 

    [Header("Engine Parameters")]
    public int SolverIterations     = 32; // number of constraint solver iterations
    public int VelocityIterations   = 32; // number of velocity solver iterations (only PhysX)
    public float FixedTimestep      = 1 / 30f; // timestep of one frame
    public int SubSteps             = 10; // number of substeps to compute for added quality
   
    [Header("Objects Parameters")]
    public ObjectsType ObjectsType;
    public GameObject CubePrefab;
    public GameObject SpherePrefab;
    public GameObject ArenaPrefab;
    public float ObjectScale            = 1.0f; // scale applied to each object
    public float ObjectDensity          = 0.1f; // amount of the world volume that is occupied by objects
    public float ObjectMass             = 1.0f; // per object mass
    public float NormMaxInitialPosition = 0.90f; // size of the 'ball' of objects at frame 1, relative to the WorldSideLength
    public float NormMaxInitialVelocity = 0.75f; // initial speed of objects,  relative to the WorldSideLength
    public float NormGravity            = 0.0981f; // gravity force,  relative to the WorldSideLength


    // Desired world volume, respecting the density parameter
    public float WorldVolume => this.ObjectsVolume / this.ObjectDensity;
    // Total volume occupied by objects
    public float ObjectsVolume => Mathf.Pow(this.ObjectScale, 3) * this.N;
    // Side length of the world's cube
    public float WorldSideLength => Mathf.Pow(this.WorldVolume, 1.0f / 3.0f);

    // Absolute max initial position
    public float MaxInitialPosition => this.WorldSideLength * this.NormMaxInitialPosition / 2;
    // Absolute max initial velocity
    public float MaxInitialVelocity => this.NormMaxInitialVelocity * this.WorldSideLength;
    // Absolute world gravity
    public float Gravity => this.WorldSideLength * this.NormGravity;

    // Timestep of one substep
    public float SubStepTime => this.FixedTimestep / this.SubSteps;


    public SimulationParameters ShallowCopy() {
        SimulationParameters output = new SimulationParameters();

        FieldInfo[] fieldInfo = typeof(SimulationParameters).GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
        foreach (FieldInfo field in fieldInfo) {
            field.SetValue(output, field.GetValue(this));
        }

        return output;
    }

    public override string ToString() {
        return $"{this.Simulation} {this.ObjectsType} N{this.N} F{this.Frames}frames";
    }
}