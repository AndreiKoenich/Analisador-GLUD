import { AutomataVertex, InputFileInfo } from "./classes"
import { minorProductionRuleSize } from "./constants"
import { isAlphaOrDigit } from "./utils"
export { minorProductionRuleSize} from "./constants"

export function buildNonDeterministicAutomata (grammarInfo: InputFileInfo) : AutomataVertex[] {
    let vertexesNDA: AutomataVertex[] = []

    for (let i: number = 0; i < grammarInfo.variables.length; i++)
        vertexesNDA.push(new AutomataVertex(grammarInfo.variables[i],[], false))

    for (let i: number = 0; i < grammarInfo.productionRules.length; i++) {

        if (grammarInfo.productionRules[i].length >= minorProductionRuleSize && isAlphaOrDigit(grammarInfo.productionRules[i][5]))
            vertexesNDA[grammarInfo.variables.indexOf(grammarInfo.productionRules[i][0])].edges.push(grammarInfo.productionRules[i].substring(5))

        else 
            vertexesNDA[grammarInfo.variables.indexOf(grammarInfo.productionRules[i][0])].isFinalState = true
    }

    return vertexesNDA
}