source_filename = "../../examples/scope_test.cs"
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
@g0 = global i32 243
@g1 = global i32 5
@g2 = global i32 0
define dso_local i32 @func(i32 noundef %v3, double noundef %v5, i8 noundef %v7) #0 {
%v9 = alloca i8
store i8 %v7, i8* %v9
%v10 = alloca double
store double %v5, double* %v10
%v11 = alloca i32
store i32 %v3, i32* %v11
%v12 = alloca [11 x i8]
%v13 = bitcast [11 x i8]* %v12 to i8*
call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %v13, i8* align 1 getelementptr inbounds ([11 x i8], [11 x i8]* @__const.func.funcprint, i32 0, i32 0), i64 11, i1 false)
%v15 = getelementptr inbounds [11 x i8], [11 x i8]* %v12, i64 0, i64 0
%v16 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i64 0, i64 0), i8* noundef %v15)
%v17 = alloca i32
store i32 151, i32* %v17
%v18 = alloca i32
%v19 = load i32, i32* %v11
store i32 %v19, i32* %v18
%v20 = load i32, i32* %v17
%v21 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v20)
%v22 = load i32, i32* %v11
%v23 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v22)
%v24 = load double, double* %v10
%v25 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i64 0, i64 0), double noundef %v24)
%v26 = load i8, i8* %v9
%v27 = sext i8 %v26 to i32
%v28 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0), i32 noundef %v27)
%v29 = alloca [9 x i8]
%v30 = bitcast [9 x i8]* %v29 to i8*
call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %v30, i8* align 1 getelementptr inbounds ([9 x i8], [9 x i8]* @__const.func.funcprintend, i32 0, i32 0), i64 9, i1 false)
%v32 = getelementptr inbounds [9 x i8], [9 x i8]* %v29, i64 0, i64 0
%v33 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i64 0, i64 0), i8* noundef %v32)
ret i32 0
}
define dso_local double @main() #0 {
%v35 = alloca i32
store i32 3, i32* %v35
%v36 = alloca i32
store i32 15, i32* %v36
%v37 = alloca i32
store i32 5, i32* %v37
%v38 = alloca i8
store i8 65, i8* %v38
%v39 = alloca double
store double 24.2, double* %v39
%v40 = load i32, i32* %v35
%v41 = add i32 %v40, 1
store i32 %v41, i32* %v36
br label %v42
v42:
%v45 = load i32 , i32* %v36
%v44 = load i32, i32* %v35
%v46 = icmp slt i32 %v44, %v45
br i1 %v46, label %v47, label %v43
v47:
%v49 = alloca [9 x i8]
%v50 = bitcast [9 x i8]* %v49 to i8*
call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %v50, i8* align 1 getelementptr inbounds ([9 x i8], [9 x i8]* @__const.main.message, i32 0, i32 0), i64 9, i1 false)
%v52 = getelementptr inbounds [9 x i8], [9 x i8]* %v49, i64 0, i64 0
%v53 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i64 0, i64 0), i8* noundef %v52)
%v54 = load i32, i32* @g0
%v55 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v54)
%v56 = alloca i32
store i32 1, i32* %v56
%v57 = alloca i32
store i32 2, i32* %v57
%v58 = alloca i32
store i32 3, i32* %v58
%v59 = alloca i32
store i32 1, i32* %v59
%v60 = load i32, i32* %v35
%v61 = icmp eq i32 %v60, 3
br i1 %v61, label %v62, label %v63
v62:
store i32 5, i32* %v57
%v64 = alloca i32
store i32 4, i32* %v64
%v65 = alloca i32
store i32 6, i32* %v65
br label %v66
v66:
%v69 = load i32 , i32* %v64
%v68 = load i32, i32* %v65
%v70 = icmp sgt i32 %v68, %v69
br i1 %v70, label %v71, label %v67
v71:
%v73 = load i32, i32* %v37
%v74 = load i32, i32* %v65
%v75 = add i32 %v73, %v74
store i32 %v75, i32* %v37
%v76 = alloca [8 x i8]
%v77 = bitcast [8 x i8]* %v76 to i8*
call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %v77, i8* align 1 getelementptr inbounds ([8 x i8], [8 x i8]* @__const.main.messagefor, i32 0, i32 0), i64 8, i1 false)
%v79 = getelementptr inbounds [8 x i8], [8 x i8]* %v76, i64 0, i64 0
%v80 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i64 0, i64 0), i8* noundef %v79)
%v81 = load i32, i32* %v37
%v82 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v81)
br label %v72
v72:
%v83 = load i32, i32* %v65
%v84 = sub i32 %v83, 1
store i32 %v84, i32* %v65
br label %v66
v67:
store i32 4444, i32* %v59
br label %v63
v63:
%v85 = alloca [8 x i8]
%v86 = bitcast [8 x i8]* %v85 to i8*
call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %v86, i8* align 1 getelementptr inbounds ([8 x i8], [8 x i8]* @__const.main.message1, i32 0, i32 0), i64 8, i1 false)
%v88 = getelementptr inbounds [8 x i8], [8 x i8]* %v85, i64 0, i64 0
%v89 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i64 0, i64 0), i8* noundef %v88)
%v90 = load i32, i32* %v59
%v91 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v90)
%v92 = alloca i32
store i32 2, i32* %v92
store i32 5, i32* %v57
br label %v48
v48:
%v93 = load i32, i32* %v35
%v94 = add i32 %v93, 1
store i32 %v94, i32* %v35
br label %v42
v43:
%v95 = alloca [9 x i8]
%v96 = bitcast [9 x i8]* %v95 to i8*
call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %v96, i8* align 1 getelementptr inbounds ([9 x i8], [9 x i8]* @__const.main.message3, i32 0, i32 0), i64 9, i1 false)
%v98 = getelementptr inbounds [9 x i8], [9 x i8]* %v95, i64 0, i64 0
%v99 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i64 0, i64 0), i8* noundef %v98)
%v100 = load i32, i32* @g1
%v101 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v100)
br label %v102
v102:
%v104 = load i32, i32* %v35
%v105 = icmp slt i32 %v104, 51
br i1 %v105, label %v106, label %v103
v106:
%v108 = alloca i32
store i32 6, i32* %v108
%v109 = alloca i32
store i32 7, i32* %v109
store i32 8, i32* @g1
br label %v107
v107:
%v110 = load i32, i32* %v35
%v111 = add i32 %v110, 1
store i32 %v111, i32* %v35
br label %v102
v103:
%v113 = getelementptr inbounds [9 x i8], [9 x i8]* %v95, i64 0, i64 0
%v114 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i64 0, i64 0), i8* noundef %v113)
%v115 = load i32, i32* @g1
%v116 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v115)
%v117 = load i32, i32* %v35
%v118 = icmp eq i32 %v117, 2
br i1 %v118, label %v119, label %v120
v119:
%v122 = alloca i32
store i32 242, i32* %v122
br label %v121
v120:
%v123 = alloca i32
store i32 2, i32* %v123
br label %v121
v121:
%v124 = load i32, i32* %v36
%v125 = load double, double* %v39
%v126 = load i8, i8* %v38
call i32 @func(i32 noundef %v124, double noundef %v125, i8 noundef %v126)
%v127 = alloca [9 x i8]
%v128 = bitcast [9 x i8]* %v127 to i8*
call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %v128, i8* align 1 getelementptr inbounds ([9 x i8], [9 x i8]* @__const.main.message4, i32 0, i32 0), i64 9, i1 false)
%v130 = getelementptr inbounds [9 x i8], [9 x i8]* %v127, i64 0, i64 0
%v131 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i64 0, i64 0), i8* noundef %v130)
%v132 = load i32, i32* @g0
%v133 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %v132)
ret double 0.0
}
@__const.func.funcprint = private unnamed_addr constant [11 x i8] c"func START\00"

@__const.func.funcprintend = private unnamed_addr constant [9 x i8] c"func END\00"

@__const.main.message = private unnamed_addr constant [9 x i8] c"Global a\00"

@__const.main.message1 = private unnamed_addr constant [8 x i8] c"Local j\00"

@__const.main.message3 = private unnamed_addr constant [9 x i8] c"Global j\00"

@__const.main.message4 = private unnamed_addr constant [9 x i8] c"Global a\00"

@__const.main.messagefor = private unnamed_addr constant [8 x i8] c"Local l\00"

declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #2
declare i32 @printf(i8* noundef, ...) #1
attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { argmemonly nofree nounwind willreturn }
