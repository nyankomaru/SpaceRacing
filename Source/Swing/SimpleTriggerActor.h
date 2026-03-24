#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SimpleTriggerActor.generated.h"

// 前方宣言
class USoundBase;

/**
 * 汎用トリガーActor
 *
 * 主な役割：
 * ・プレイヤーや他Actorとの重なり判定を行う
 * ・トリガー発火時のイベントをBlueprintへ渡す
 * ・必要に応じてトリガーSEを再生する
 *
 * 用途例：
 * ・ゴール地点
 * ・イベント発火ポイント
 * ・演出開始用トリガー
 */
UCLASS()
class SWING_API ASimpleTriggerActor : public AActor
{
	GENERATED_BODY()

public:
	ASimpleTriggerActor();

protected:
	// =========================
	// Trigger Setting
	// =========================

	/** トリガー判定用のBox */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
	UBoxComponent* TriggerBox;

	/** 1回だけ発火するか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	bool bTriggerOnce = true;

	/** すでに発火済みか */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
	bool bTriggered = false;

	// =========================
	// Trigger Audio
	// =========================

	/** トリガー発火時に鳴らすSE。未設定なら再生しない */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Trigger")
	USoundBase* TriggerSE = nullptr;

	/** トリガーSEの音量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Trigger", meta = (ClampMin = "0.0"))
	float TriggerSEVolume = 1.0f;

	/** トリガーSEのピッチ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Trigger", meta = (ClampMin = "0.0"))
	float TriggerSEPitch = 1.0f;

protected:
	// =========================
	// Overlap Callback
	// =========================

	/** Overlap開始時に呼ばれる */
	UFUNCTION()
	void OnTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/** Overlap終了時に呼ばれる */
	UFUNCTION()
	void OnTriggerEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

protected:
	// =========================
	// Blueprint Event
	// =========================

	/**
	 * トリガー発火時のイベント
	 * Blueprint側で演出や遷移処理を実装する
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Trigger")
	void OnTriggered(AActor* OverlappingActor);

	/**
	 * トリガー解除時のイベント
	 * 必要に応じてBlueprint側で実装する
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Trigger")
	void OnTriggerReleased(AActor* OverlappingActor);
};