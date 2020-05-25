// SimulationGenerator - Runner.cs (2019)
// Copyright: Ygor Rebouças Serpa


using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;


/// <summary>
/// This script runs a set of 'SimulationParameters' simulation parameters.
/// Usually, these 'SimulationParameters' are generated from a 'Startup' script
/// </summary>
public class Runner : MonoBehaviour {
    public Wizard wizard;
    public GUISkin guiSkin;
    public bool showDebugInfo;
    public SimulationParameters[] parameters;

    private SimulationParameters currentParameters;
    private Simulation           currentSimulation;
    private int                  currentFrame;
    private float                timeAtStart;
    private float                currentFrameTimeTaken;
    private float                averageFrameTime;


    private void Start() {
        Camera.allCameras[0].rect = new Rect(this.showDebugInfo ? 0.3f : 0, 0, 1, 1);

        this.StartCoroutine(this.RunSimulations());
    }

    private IEnumerator RunSimulations() {
        foreach (SimulationParameters parameter in this.parameters) {
            this.currentParameters = parameter;
            using (Simulation s = Factory.GetSimulation(parameter.Simulation)) {
                this.currentSimulation = s;
                this.timeAtStart = Time.realtimeSinceStartup;

                s.Start(parameter);
//////////////
// EX JOBB ///////////////////////////////////////////////////////////////
                SetUpStatickObjekts(parameter);
//////////////////////////////////////////////////////////////////////////

                for (int i = 0; i < parameter.Frames; i++) {
                    this.currentFrame = i;

                    float timeTaken = Stopwatch.MeasureAction(() => s.Step(i));
                    // Weighted exponential moving average
                    const float alpha           = 0.20f;
                    this.averageFrameTime       = alpha * timeTaken + (1 - alpha) * this.currentFrameTimeTaken;
                    this.currentFrameTimeTaken  = timeTaken;
                    yield return null;
                }

                s.Stop();
            }
        }

        // If there is a wizard, go back to it
        this.wizard?.gameObject.SetActive(true);
        UnityEngine.Object.Destroy(this.gameObject);
    }


//////////////
// EX JOBB ///////////////////////////////////////////////////////////////
    private void SetUpStatickObjekts( SimulationParameters parm)
    {
        
        int staticCount = (int)(this.currentSimulation.Bodies.Count * parm.Ps);
        int objectCount = this.currentSimulation.Bodies.Count;

        Material staticMaterial = new Material(this.currentSimulation.Bodies[0].Renderer.material)
        {
            color = new Color(1f,0f, 0f) // new Color(0, 237 / 255f, 177 / 255f) / 4
        };

        for (int i = 0; i < staticCount; i++)
        {
           int selected = UnityEngine.Random.Range(0, objectCount);

            this.currentSimulation.Bodies[selected].GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeAll;
            this.currentSimulation.Bodies[selected].Renderer.sharedMaterial               = staticMaterial;
            this.currentSimulation.Bodies[selected].isStatick                             = true;

            PhysicsBody temp = this.currentSimulation.Bodies[selected];
            this.currentSimulation.Bodies.RemoveAt(selected);
            this.currentSimulation.Bodies.Add(temp);
            --objectCount; 
        }



    }
//////////////////////////////////////////////////////////////////////////

    private void OnGUI() {
        if (this.currentSimulation.Parameters.RecordMovie) {
            return;
        }

        GUI.skin = this.guiSkin;
        GUI.backgroundColor = new Color(0.12f, 0.12f, 0.12f, 0.98f);

        GUILayout.BeginArea(new Rect(Screen.width * 0.01f, Screen.height * 0.05f, Screen.width / 4, Screen.height));
        GUILayout.BeginVertical(GUI.skin.box);
        foreach (FieldInfo fields in typeof(SimulationParameters).GetFields()) {
            GUILayout.Label($"{fields.Name.PadLeft(22)}: {fields.GetValue(this.currentParameters)}");
        }

        GUILayout.Space(20f);
        foreach (PropertyInfo properties in typeof(SimulationParameters).GetProperties()) {
            GUILayout.Label($"{properties.Name.PadLeft(22)}: {properties.GetValue(this.currentParameters)}");
        }

        GUILayout.Space(20f);
        float totalTimeTaken = (Time.realtimeSinceStartup - this.timeAtStart) / 60f;
        float fps = 1.0f / this.averageFrameTime;
        float eta = (this.currentParameters.Frames - this.currentFrame) * this.averageFrameTime / 60f;
        GUILayout.Label($"{"Current Frame".PadLeft(22)}: {this.currentFrame} / {this.currentParameters.Frames}");

        // weighted exponential moving average
        GUILayout.Label($"{"WEMA frame time".PadLeft(22)}: {this.averageFrameTime:###0.000} ({fps:##0.0} fps)");
        GUILayout.Label($"{"Elapsed + Remaining".PadLeft(22)}: " + $"{totalTimeTaken:####0.0} + {eta:####0.0} = {totalTimeTaken + eta:####0.0} minutes");


        GUILayout.Space(20f);
        this.currentSimulation?.OnGUI();

        GUILayout.EndVertical();
        GUILayout.EndArea();
    }

    private void OnDestroy() {
        this.currentSimulation?.Dispose();
    }
}