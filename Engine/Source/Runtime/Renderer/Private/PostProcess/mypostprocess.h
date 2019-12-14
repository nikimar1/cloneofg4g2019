// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

/*=============================================================================
	mypostprocess.h: my post process implementation for assn7
=============================================================================*/

#pragma once

#include "CoreMinimal.h"
#include "RendererInterface.h"
#include "PostProcess/RenderingCompositionGraph.h"

// ePId_Input0: Full Res Scene Color
// ePId_Input1: Full Res Scene Depth
// derives from TRenderingCompositePassBase<InputCount, OutputCount> 
class FRCPassPostProcessMyProcess : public TRenderingCompositePassBase<2, 1>
{
public:

	FRCPassPostProcessMyProcess();
	
	
	
	//public:
	//FRCPassPostProcessVelocityFlatten();

	// interface FRenderingCompositePass ---------
	virtual void Process(FRenderingCompositePassContext& Context) override;
	virtual void Release() override { delete this; }
	virtual FPooledRenderTargetDesc ComputeOutputDesc(EPassOutputId InPassOutputId) const override;

	// -------------------------------------------

	//static FIntPoint ComputeThreadGroupCount(FIntPoint PixelExtent);
	
	

	// @param InQuality 0xffffffff to visualize, 0:off(no shader is used), 1:low, 2:medium, 3:high, 4:very high
	//FRCPassPostProcessMotionBlur( uint32 InQuality, int32 InPass, bool InIsComputePass )
	//	: Quality(InQuality)
	//	, Pass(InPass)
	//{
	//	bIsComputePass = InIsComputePass;
	//	bPreferAsyncCompute = false;

		// internal error
	//	check(Quality >= 1 && Quality <= 4);
	//}

	// interface FRenderingCompositePass ---------
	//virtual void Process(FRenderingCompositePassContext& Context) override;
	//virtual void Release() override { delete this; }
	//virtual FPooledRenderTargetDesc ComputeOutputDesc(EPassOutputId InPassOutputId) const override;

	//virtual FComputeFenceRHIParamRef GetComputePassEndFence() const override { return AsyncEndFence; }

	// 1:low, 2:medium, 3:high, 4: very high
	//uint32	Quality;
	//int32	Pass;

//private:
//	template <typename TRHICmdList>
	//void DispatchCS(TRHICmdList& RHICmdList, FRenderingCompositePassContext& Context, const FIntPoint& DestSize, const FIntRect& DestRect, FUnorderedAccessViewRHIParamRef DestUAV, float Scale, uint32 InnerLoopConfig);
	
//	FComputeFenceRHIRef AsyncEndFence;
};


// ePId_Input0: Full Res Scene Color
// ePId_Input1: Full Res Scene Depth
// ePId_Input2: Full Res velocity input
// derives from TRenderingCompositePassBase<InputCount, OutputCount> 
//class FRCPassPostProcessVisualizeMotionBlur : public TRenderingCompositePassBase<3, 1>
//{
//public:
	// interface FRenderingCompositePass ---------
//	virtual void Process(FRenderingCompositePassContext& Context) override;
//	virtual void Release() override { delete this; }
//	virtual FPooledRenderTargetDesc ComputeOutputDesc(EPassOutputId InPassOutputId) const override;
//};
