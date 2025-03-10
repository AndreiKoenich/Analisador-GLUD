export class InputFileInfo {
    grammarName: string = ""
    variables: string[] = []
    terminals: string[] = []
    productionsName: string = ""
    initialVariable: string = ""
    productionRules: string[] = []
}

export class AutomataVertex {
    variable: string = ""
    edges: string[] = []
    neighbors: AutomataVertex[] = []
    constructor (variable: string, edges: string[], neighbors: AutomataVertex[]) {
        this.variable = variable
        this.edges = edges
        this.neighbors = neighbors
    }
}