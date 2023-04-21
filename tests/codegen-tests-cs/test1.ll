source_filename = "../../tests/codegen-tests-cs/test1.cs"
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
define dso_local i32 @func(i32 noundef %v0) #0 {
%v2 = alloca i32
store i32 %v0, i32* %v2
%v3 = load i32, i32* %v2
%v4 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v3)
%v5 = load i32, i32* %v2
%v6 = add i32 %v5, 2022
store i32 %v6, i32* %v2
%v7 = load i32, i32* %v2
%v8 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v7)
%v9 = load i32, i32* %v2
ret i32 %v9
}
define dso_local i32 @main() #0 {
%v10 = alloca i32
store i32 10, i32* %v10
%v11 = alloca i32
%v12 = load i32, i32* %v10
store i32 %v12, i32* %v11
%v13 = alloca i32
%v14 = load i32, i32* %v10
%v15 = load i32, i32* %v11
%v16 = add i32 %v14, %v15
store i32 %v16, i32* %v13
%v17 = load i32, i32* %v13
%v18 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v17)
%v19 = alloca double
store double 2.1, double* %v19
%v20 = alloca double
%v21 = load double, double* %v19
store double %v21, double* %v20
%v22 = alloca double
%v23 = load double, double* %v19
%v24 = load double, double* %v20
%v25 = fadd double %v23, %v24
store double %v25, double* %v22
%v26 = load double, double* %v22
%v27 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i64 0, i64 0), double noundef %v26)
%v28 = load i32, i32* %v13
%v29 = sub i32 %v28, 12
store i32 %v29, i32* %v13
%v30 = load i32, i32* %v13
%v31 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v30)
%v32 = load double, double* %v20
%v33 = fsub double %v32, 14.7
store double %v33, double* %v22
%v34 = load double, double* %v22
%v35 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i64 0, i64 0), double noundef %v34)
%v36 = load i32, i32* %v13
%v37 = sdiv i32 %v36, 2
store i32 %v37, i32* %v13
%v38 = load i32, i32* %v13
%v39 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v38)
%v40 = load i32, i32* %v13
%v41 = srem i32 %v40, 3
store i32 %v41, i32* %v13
%v42 = load i32, i32* %v13
%v43 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v42)
store i32 84, i32* %v13
%v44 = load i32, i32* %v13
%v45 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v44)
%v46 = load i32, i32* %v10
%v47 = add i32 42, %v46
store i32 %v47, i32* %v13
%v48 = load i32, i32* %v13
%v49 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v48)
%v50 = load i32, i32* %v13
%v51 = icmp sgt i32 %v50, 51
br i1 %v51, label %v52, label %v53
v52:
store i32 0, i32* %v13
%v55 = load i32, i32* %v13
%v56 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v55)
br label %v54
v53:
store i32 1, i32* %v13
%v57 = load i32, i32* %v13
%v58 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v57)
br label %v54
v54:
%v59 = alloca i32
store i32 0, i32* %v59
br label %v60
v60:
%v62 = load i32, i32* %v59
%v63 = icmp slt i32 %v62, 5
br i1 %v63, label %v64, label %v61
v64:
%v66 = load i32, i32* %v59
%v67 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v66)
br label %v65
v65:
%v68 = load i32, i32* %v59
%v69 = add i32 %v68, 1
store i32 %v69, i32* %v59
br label %v60
v61:
%v70 = alloca i32
store i32 0, i32* %v70
br label %v71
v71:
%v73 = load i32, i32* %v70
%v74 = icmp slt i32 %v73, 5
br i1 %v74, label %v75, label %v72
v75:
%v77 = load i32, i32* %v70
%v78 = add i32 %v77, 14
store i32 %v78, i32* %v70
%v79 = load i32, i32* %v70
%v80 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v79)
br label %v76
v76:
%v81 = load i32, i32* %v70
%v82 = add i32 %v81, 1
store i32 %v82, i32* %v70
br label %v71
v72:
%v83 = alloca i32
%v84 = load i32, i32* %v13
%v85 = call i32 @func(i32 noundef %v84)
store i32 %v85, i32* %v83
%v86 = load i32, i32* %v83
%v87 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v86)
%v88 = load i32, i32* %v10
ret i32 %v88
}
declare i32 @printf(i8* noundef, ...) #1
attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { argmemonly nofree nounwind willreturn }
