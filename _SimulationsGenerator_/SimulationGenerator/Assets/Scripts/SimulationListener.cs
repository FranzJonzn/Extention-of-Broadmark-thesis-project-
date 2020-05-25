﻿// SimulationGenerator - SimulationListener.cs (2019)
// Copyright: Ygor Rebouças Serpa


using System;
using System.Collections.Generic;


public abstract class SimulationListener {
    public abstract void OnStart(Simulation simulation);
	//////////////
// EX JOBB ///////////////////////////////////////////////////////////////
    public abstract void OnStart(Simulation simulation,string path);
////////////////////////////////////////////////////////////////////////
    public abstract void OnStep(Simulation simulation);
    public abstract void OnStop(Simulation simulation);
}