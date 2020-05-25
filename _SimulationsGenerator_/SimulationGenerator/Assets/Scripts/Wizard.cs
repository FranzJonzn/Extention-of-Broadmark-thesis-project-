using System;
using System.IO;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
using Object = UnityEngine.Object;


public class Wizard : MonoBehaviour {
    // Used to generate scene and object type toggles
    public HorizontalLayoutGroup ScenesComponent;
    public HorizontalLayoutGroup ObjectTypeComponent;

    // Components to fetch results from
    public TMP_InputField OutputPath;
    public Toggle RecordScene;
    public Toggle RecordMovie;
    public TMP_Dropdown EnginesDropdown;
    public List<Toggle> SceneToggles;
    public List<Toggle> ObjectTypeToggles;
    public TMP_InputField NumberOfObjects;

///////////////
/// EX_Jobb ////////////////////////////////
    public TMP_InputField ProcentStatic;  //
    public TMP_InputField redunentTest;   //
    public Toggle UseExjobTest;           //
////////////////////////////////////////////
    public TMP_InputField Frames;
    public TMP_InputField SolverIterations;
    public TMP_InputField Timestep;
    public TMP_InputField Substeps;
    public TMP_InputField Density;
    public TMP_InputField Gravity;

    // Utilities
    public TMP_Text Warning;
    public GameObject StartupPrefab;
    public GameObject TooglePrefab;
	
	
//////////////////
// EX_Jobb    ///////////////////////////////
    //  intervalls                         //
    public Toggle intervallToggle1;        //
    public TMP_InputField interStart1;     //
    public TMP_InputField interEnd1;       //
    public TMP_InputField interInterval1;  //
    public Toggle intervallToggle2;        //
    public TMP_InputField interStart2;     //
    public TMP_InputField interEnd2;       //
    public TMP_InputField interInterval2;  //
    public Toggle intervallToggle3;        //  
    public TMP_InputField interStart3;     //
    public TMP_InputField interEnd3;       //  
    public TMP_InputField interInterval3;  //
/////////////////////////////////////////////

    // Filling the engines dropdown, scenes and object types
    private void Start() {
        List<string> data = new List<string>();
        foreach (PhysicsEngineType engine in Enum.GetValues(typeof(PhysicsEngineType))) {
            data.Add(engine.ToString());
        }

        this.EnginesDropdown.AddOptions(data);

        this.SceneToggles = new List<Toggle>();
        foreach (SimulationType scene in Enum.GetValues(typeof(SimulationType))) {
            GameObject go = Object.Instantiate(this.TooglePrefab);
            go.transform.SetParent(this.ScenesComponent.transform);
            go.GetComponentInChildren<Text>().text = scene.ToString();
            this.SceneToggles.Add(go.GetComponent<Toggle>());
        }

        this.ObjectTypeToggles = new List<Toggle>();
        foreach (ObjectsType objectType in Enum.GetValues(typeof(ObjectsType))) {
            GameObject go = Object.Instantiate(this.TooglePrefab);
            go.transform.SetParent(this.ObjectTypeComponent.transform);
            go.GetComponentInChildren<Text>().text = objectType.ToString();
            this.ObjectTypeToggles.Add(go.GetComponent<Toggle>());
        }

        // By default, only enables cubes
        for (int i = 1; i < this.ObjectTypeToggles.Count; i++) {
            this.ObjectTypeToggles[i].isOn = false;
        }
///////////////
/// EX_Jobb ////////////////
        SetModToExjobb(); //
////////////////////////////

    }
    ///////////////
    // EX_Jobb /////////////////////////////////////////////////////////////////////////////
    
/// <summary>
/// Dimar ut alla exjobb fält när xjobb inte kommmer användas
/// </summary>
    public void SetModToExjobb()                                                       
    {
        NumberOfObjects.interactable = !UseExjobTest.isOn;

        intervallToggle1.interactable   = UseExjobTest.isOn;
        interStart1.interactable        = UseExjobTest.isOn;
        interEnd1.interactable          = UseExjobTest.isOn;
        interInterval1.interactable     = UseExjobTest.isOn;
        intervallToggle2.interactable   = UseExjobTest.isOn;
        interStart2.interactable        = UseExjobTest.isOn;
        interEnd2.interactable          = UseExjobTest.isOn;
        interInterval2.interactable     = UseExjobTest.isOn;
        intervallToggle3.interactable   = UseExjobTest.isOn;
        interStart3.interactable        = UseExjobTest.isOn;
        interEnd3.interactable          = UseExjobTest.isOn;
        interInterval3.interactable     = UseExjobTest.isOn;

        redunentTest.interactable = UseExjobTest.isOn;

        if (UseExjobTest.isOn)
        {
            ProcentStatic.text = "0,20,40,60,80,100";
        }
        else
        {
            string text = ProcentStatic.text;
            string[] tokens = text.Split(',');
            foreach(string t in tokens)
            {
                Debug.Log(tokens[0]);
            }

            ProcentStatic.text = tokens[0];
            ProcentStatic.textComponent.enabled = false;
            ProcentStatic.textComponent.enabled = true;
        }
    }
    /// <summary>
    /// Återställer andelen statiska objekt fälltet när exjobb stängsav.
    /// </summary>
    public void ChangeProsentStaticText()
    {
        if (!UseExjobTest.isOn)
        {
            string[] tokens = ProcentStatic.text.Split(',');
            ProcentStatic.text = tokens[0];
            ProcentStatic.textComponent.enabled = false;
            ProcentStatic.textComponent.enabled = true;
        }
    
    }
///////////////////////////////////////////////////////////////////////////////////////////////////
    
	public void ParseAndStart() {
        try {
            #region IO Settings
            string path = this.OutputPath.text;
            if (!Directory.Exists(path)) {
                this.Warning.text = "The specified path does not exists!";
                return;
            }

            bool recordSimulation = this.RecordScene.isOn;
            bool recordMovie      = this.RecordMovie.isOn;
///////////////
// EX_Jobb ///////////////////////////////////////////////
            bool useExjobTest     = UseExjobTest.isOn;  //
//////////////////////////////////////////////////////////
            #endregion

            #region Engine
            string engineName = this.EnginesDropdown.options[this.EnginesDropdown.value].text;
            if (!Enum.TryParse<PhysicsEngineType>(engineName, out PhysicsEngineType engine)) {
                this.Warning.text = "The selected engine is invalid!";
                return;
            }
            #endregion

            #region Scenes
            List<SimulationType> scenes = new List<SimulationType>();
            for (int i = 0; i < this.SceneToggles.Count; i++) {
                if (this.SceneToggles[i].isOn) {
                    scenes.Add((SimulationType)i);
                }
            }

            if (scenes.Count == 0) {
                this.Warning.text = "At least one scene must be selected!";
                return;
            }
            #endregion

            #region Object Types
            List<ObjectsType> objectTypes = new List<ObjectsType>();
            for (int i = 0; i < this.ObjectTypeToggles.Count; i++) {
                if (this.ObjectTypeToggles[i].isOn) {
                    objectTypes.Add((ObjectsType)i);
                }
            }

            if (objectTypes.Count == 0) {
                this.Warning.text = "At least one object type must be selected!";
                return;
            }
            #endregion

            #region Number of Objects
            List<int> numberOfObjects = new List<int>();
            string numberOfObjectsText = NumberOfObjects.text;
            string[] tokens = numberOfObjectsText.Split(',');
            for (int i = 0; i < tokens.Length; i++) {
                string token = tokens[i];
                if (!int.TryParse(token, out int n)) {
                    this.Warning.text = $"Could not read '# of Objects' position {i + 1}: {token}";
                    return;
                }

                if (n <= 0) {
                    this.Warning.text = $"'# of Objects' position {i + 1} must be greater than zero!";
                    return;
                }

                numberOfObjects.Add(n * 1000);// 
            }

            if (numberOfObjects.Count == 0) {
                this.Warning.text = "At least one '# of Objects' mut be given!";
                return;
            }
            #endregion

///////////////
// EX_Jobb ////////////////////////////////////////////////////////////        
            #region Procent Static Objekts
            List<float> numberOfprocentRuns = new List<float>();
            string numberOfprocentRunsText = ProcentStatic.text;
            tokens = numberOfprocentRunsText.Split(',');

            for(int i = 0; i < tokens.Length; i++)
            {
                string token = tokens[i];
                if (!int.TryParse(token, out int n))
                {
                    this.Warning.text = $"Could not read '# procent static' position {i + 1}: {token}";
                    return;
                }
                if (n < 0)
                {
                    this.Warning.text = "Cant be negative procentage!";
                    return;
                }
                else if (n > 100)
                {
                    this.Warning.text = "cant be more than 100% !";
                    return;
                }


                float res = ((float)n) / 100.0f;


                numberOfprocentRuns.Add(res);
            }

           
           
            #endregion

            #region redundetTests
            if (!int.TryParse(redunentTest.text, out int redundansyTests))
            {
                this.Warning.text = $"Could not read :" + ProcentStatic.text;
                return;
            }

            #endregion

            #region Intervalls
            List<Vector3Int> intervals = new List<Vector3Int>();
            if (intervallToggle1.isOn)
            {
                if (!int.TryParse(interStart1.text, out int start))
                {
                    this.Warning.text = $"Could not read :" + interStart1.text;
                    return;
                }
                if (!int.TryParse(interEnd1.text, out int end))
                {
                    this.Warning.text = $"Could not read :" + interEnd1.text;
                    return;
                }
                if (!int.TryParse(interInterval1.text, out int interval))
                {
                    this.Warning.text = $"Could not read :" + interInterval1.text;
                    return;
                }

                intervals.Add(new Vector3Int(start, end, interval));
            }
            if (intervallToggle2.isOn)
            {
                if (!int.TryParse(interStart2.text, out int start))
                {
                    this.Warning.text = $"Could not read :" + interStart2.text;
                    return;
                }
                if (!int.TryParse(interEnd2.text, out int end))
                {
                    this.Warning.text = $"Could not read :" + interEnd2.text;
                    return;
                }
                if (!int.TryParse(interInterval2.text, out int interval))
                {
                    this.Warning.text = $"Could not read :" + interInterval2.text;
                    return;
                }

                intervals.Add(new Vector3Int(start, end, interval));
            }
            if (intervallToggle3.isOn)
            {
                if (!int.TryParse(interStart3.text, out int start))
                {
                    this.Warning.text = $"Could not read :" + interStart3.text;
                    return;
                }
                if (!int.TryParse(interEnd3.text, out int end))
                {
                    this.Warning.text = $"Could not read :" + interEnd3.text;
                    return;
                }
                if (!int.TryParse(interInterval3.text, out int interval))
                {
                    this.Warning.text = $"Could not read :" + interInterval3.text;
                    return;
                }

                intervals.Add(new Vector3Int(start, end, interval));
            }

            if(intervals.Count == 0 && useExjobTest)
            {
                this.Warning.text = "Must have atleast on intervall ";
            }

            #endregion
//////////////////////////////////////////////////////////////////////////////////////////////////

            #region Number of Frames
            if (!int.TryParse(this.Frames.text, out int frames)) {
                this.Warning.text = "Could not read '# of Frames': " + this.Frames.text;
                return;
            }

            if (frames <= 0) {
                this.Warning.text = "'# of Frames' must be greater than zero!";
                return;
            }
            #endregion

            #region Solver iterations
            if (!int.TryParse(this.SolverIterations.text, out int solverIterations)) {
                this.Warning.text = "Could not read '# of Solver Iterations': " + this.SolverIterations.text;
                return;
            }

            if (solverIterations <= 0) {
                this.Warning.text = "'# of Solver Iterations' must be greater than zero!";
                return;
            }
            #endregion

            #region Timestep
            if (!float.TryParse(this.Timestep.text, out float timestep)) {
                this.Warning.text = "Could not read 'Timestep': " + this.Timestep.text;
                return;
            }

            if (timestep <= 0) {
                this.Warning.text = "'Timestep' must be greater than zero!";
                return;
            }
            #endregion

            #region Substeps
            if (!int.TryParse(this.Substeps.text, out int substeps)) {
                this.Warning.text = "Could not read '# of Substeps': " + this.Substeps.text;
                return;
            }

            if (substeps <= 0) {
                this.Warning.text = "'# of Substeps' must be greater than zero!";
                return;
            }
            #endregion

            #region Density
            if (!float.TryParse(this.Density.text, out float density)) {
                this.Warning.text = "Could not read 'Density': " + this.Density.text;
                return;
            }

            if (density <= 0) {
                this.Warning.text = "'Density' must be greater than zero!";
                return;
            }
            #endregion

            #region Gravity
            if (!float.TryParse(this.Gravity.text, out float gravity)) {
                this.Warning.text = "Could not read 'Gravity': " + this.Gravity.text;
                return;
            }
            #endregion

            this.Warning.text = "Success!!!";

            GameObject go                           = Object.Instantiate(this.StartupPrefab);
            Startup instance                        = go.GetComponent<Startup>();
            instance.wizard                         = this;
            instance.OutputRoot                     = path;
            instance.Engine                         = engine;
            instance.Record                         = recordSimulation;
            instance.RecordMovie                    = recordMovie;
            instance.Frames                         = frames;
            instance.Simulations                    = scenes.ToArray();
            instance.ObjectTypes                    = objectTypes.ToArray();
            instance.Ns                             = numberOfObjects.ToArray();
///////////////
// EX_Jobb //////////////////////////////////////////////////////////////////////////////
 /*static*/ instance.Ps                             = numberOfprocentRuns.ToArray();    //
            instance.useExjobTest                   = useExjobTest;                     //
            instance.RedundancyTest                 = redundansyTests;                  //
            instance.intervals                      = intervals.ToArray();              //
//////////////////////////////////////////////////////////////////////////////////////////		 

            instance.Template.SolverIterations      = solverIterations;
            instance.Template.VelocityIterations    = solverIterations;
            instance.Template.FixedTimestep         = timestep;
            instance.Template.SubSteps              = substeps;
            instance.Template.ObjectDensity         = density;
            instance.Template.NormGravity           = gravity;

            this.gameObject.SetActive(false);
        } catch (Exception e) {
            this.Warning.text = $"Unkown '{e.GetType().Name}': {e.Message}";
            return;
        }
    }

 


}
