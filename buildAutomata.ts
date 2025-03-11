import * as Classes from "./classes"
import { isAlphaOrDigit } from "./controlGrammarFileData"

export function buildAutomata (grammarInfo: Classes.InputFileInfo) : Classes.AutomataVertex[] {
    let vertexes: Classes.AutomataVertex[] = []

    for (let i: number = 0; i < grammarInfo.variables.length; i++)
        vertexes.push(new Classes.AutomataVertex(grammarInfo.variables[i],[], false))

    for (let i: number = 0; i < grammarInfo.productionRules.length; i++) {

        if (grammarInfo.productionRules[i].length >= 6 && isAlphaOrDigit(grammarInfo.productionRules[i][5])) {

            if (grammarInfo.productionRules[i].length == 6)
                vertexes[grammarInfo.variables.indexOf(grammarInfo.productionRules[i][0])].isFinalState = true
            
            vertexes[grammarInfo.variables.indexOf(grammarInfo.productionRules[i][0])].edges.push(grammarInfo.productionRules[i].substring(5))
        }

        else {
            vertexes[grammarInfo.variables.indexOf(grammarInfo.productionRules[i][0])].isFinalState = true
        }
    }

    return vertexes
}