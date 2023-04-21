source_filename = "../../examples/nod.cs"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"
@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1
@.str.2 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@.str.3 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@.str.4 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.5 = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@.str.6 = private unnamed_addr constant [3 x i8] c"%f\00", align 1
@.str.7 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
define dso_local i32 @getnod(i32 noundef %v0, i32 noundef %v2) #0 {
%v4 = alloca i32
store i32 %v0, i32* %v4
%v5 = alloca i32
store i32 %v2, i32* %v5
%v6 = alloca i32
store i32 0, i32* %v6
%v7 = alloca i32
%v8 = load i32, i32* %v4
store i32 %v8, i32* %v7
br label %v9
v9:
%v11 = load i32, i32* %v7
%v12 = icmp sgt i32 %v11, 0
br i1 %v12, label %v13, label %v10
v13:
%v15 = load i32, i32* %v4
%v16 = load i32, i32* %v7
%v17 = srem i32 %v15, %v16
store i32 %v17, i32* %v6
%v18 = load i32, i32* %v6
%v19 = icmp eq i32 %v18, 0
br i1 %v19, label %v20, label %v21
v20:
%v22 = load i32, i32* %v5
%v23 = load i32, i32* %v7
%v24 = srem i32 %v22, %v23
store i32 %v24, i32* %v6
%v25 = load i32, i32* %v6
%v26 = icmp eq i32 %v25, 0
br i1 %v26, label %v27, label %v28
v27:
%v29 = load i32, i32* %v7
store i32 %v29, i32* %v6
store i32 0, i32* %v7
br label %v28
v28:
br label %v21
v21:
br label %v14
v14:
%v30 = load i32, i32* %v7
%v31 = sub i32 %v30, 1
store i32 %v31, i32* %v7
br label %v9
v10:
%v32 = load i32, i32* %v6
ret i32 %v32
}
define dso_local i32 @main() #0 {
%v33 = alloca i32
%v34 = alloca i32
%v35 = call i32 (i8*, ...) @scanf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4, i64 0, i64 0), i32* noundef %v33)
%v36 = call i32 (i8*, ...) @scanf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4, i64 0, i64 0), i32* noundef %v34)
%v37 = alloca i32
%v38 = load i32, i32* %v33
%v39 = load i32, i32* %v34
%v40 = call i32 @getnod(i32 noundef %v38, i32 noundef %v39)
store i32 %v40, i32* %v37
%v41 = load i32, i32* %v37
%v42 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v41)
ret i32 0
}
declare i32 @printf(i8* noundef, ...) #1
declare i32 @scanf(i8* noundef, ...) #1
attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { argmemonly nofree nounwind willreturn }
