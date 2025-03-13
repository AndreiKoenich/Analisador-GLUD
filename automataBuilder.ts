import { AutomataVertex, InputFileInfo } from "./classes"
import { isAlphaOrDigit } from "./fileDataController"

export function buildNonDeterministicAutomata (grammarInfo: InputFileInfo) : AutomataVertex[] {
    let vertexesND: AutomataVertex[] = []

    for (let i: number = 0; i < grammarInfo.variables.length; i++)
        vertexesND.push(new AutomataVertex(grammarInfo.variables[i],[], false))

    for (let i: number = 0; i < grammarInfo.productionRules.length; i++) {

        if (grammarInfo.productionRules[i].length >= 6 && isAlphaOrDigit(grammarInfo.productionRules[i][5])) {

            if (grammarInfo.productionRules[i].length == 6)
                vertexesND[grammarInfo.variables.indexOf(grammarInfo.productionRules[i][0])].isFinalState = true
            
            vertexesND[grammarInfo.variables.indexOf(grammarInfo.productionRules[i][0])].edges.push(grammarInfo.productionRules[i].substring(5))
        }

        else {
            vertexesND[grammarInfo.variables.indexOf(grammarInfo.productionRules[i][0])].isFinalState = true
        }
    }

    return vertexesND
}