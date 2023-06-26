import localsolver.modeler

with localsolver.modeler.LSPModeler() as modeler:
    module = modeler.load_module("greedy.lsp")
    module.run_main("reroll", "1,1,2,3,4", "yahtsee")
    print(list(module["reroll"]))