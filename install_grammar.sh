cd src/app/grammar
java -Xmx500M -cp "antlr-4.9-complete.jar:$CLASSPATH" org.antlr.v4.Tool -Dlanguage=Cpp Csharp.g4 -visitor