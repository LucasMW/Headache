internal fun readFromFile(path: String): String {
    val url: URL = URL(fileURLWithPath = path)
    val myString: String = String(contentsOf = url)
    return myString
}

fun main(args: Array<String>) {
    val args: List<String> = CommandLine.arguments

    if ((args.size >= 3)) {
    }
    else if ((args.size == 2)) {
        try {
            val program: String = readFromFile(path = args[1])

            println(top-level)

            // let interpreter = BrainfuckInterpreter(cells: 30000)
            //     interpreter.dbg_messages = false
            //     interpreter.setProgram(program: program)
            //     interpreter.inputRedirect = false
            //     interpreter.outputRedirect = false
            //     interpreter.execute()
        }
        catch (error: Exception) {
            println("Couldn't read file: ${args[1]}")
            exit(1)
        }
        exit(0)
    }
    else {
        println("Error!\nProgram usage is: \n${args[0]} sourcecode")
        exit(-1)
    }
}
