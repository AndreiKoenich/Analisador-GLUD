import * as Classes from "./classes"

function buildAutomata (grammarInfo: Classes.InputFileInfo) : void {
    let vertexes: Classes.AutomataVertex[] = []

    for (let i: number = 0; i < grammarInfo.variables.length; i++)
        vertexes.push(new Classes.AutomataVertex(grammarInfo.variables[i],[],[]))

    


}