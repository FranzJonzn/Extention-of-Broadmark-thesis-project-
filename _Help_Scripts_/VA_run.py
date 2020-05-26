

from VALID_bm_run import generate_jsons, run_algorithms, combinde_Validation_Results




batch_run_name = "" # use this to name this testing session. If blank, a auto-generated timestamped name will be used
broadmark_bin = "D:/SKOL_ARBETE/Kurser/ar_3/Vt/Ex_Jobb/_EX_JOBB_Broadmark_REPO/Broadmark/bin64/Broadmark_Release_x64.exe"
scenes_folder = "D:/SKOL_ARBETE/Kurser/ar_3/Vt/Ex_Jobb/_textFiller/DEL2_genomforande/Validering_utav_broadmark/valideringsSimulation/Recordings_AABBs/"
output_folder = "D:/SKOL_ARBETE/Kurser/ar_3/Vt/Ex_Jobb/_textFiller/DEL2_genomforande/Validering_utav_broadmark/ValideringenSimulationRuns/"
validation_output_folder = "D:/SKOL_ARBETE/Kurser/ar_3/Vt/Ex_Jobb/_textFiller/DEL2_genomforande/Validering_utav_broadmark/valideringenUtavBroadMark/";


algorithms = ["CGAL"]


additionalProperty = ""
additionaPropertyValues = []


#kes 100 copies of the same run

tests_folder  = generate_jsons(batch_run_name, output_folder, scenes_folder, algorithms, additionalProperty, additionaPropertyValues)
results_folder = run_algorithms(tests_folder, validation_output_folder, broadmark_bin)

combinde_Validation_Results(validation_output_folder, broadmark_bin)
