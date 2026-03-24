#include "SimpleTriggerActor.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// =========================
// コンストラクタ
// =========================

ASimpleTriggerActor::ASimpleTriggerActor()
{
	// 毎フレーム更新は不要
	PrimaryActorTick.bCanEverTick = false;

	// トリガー判定用Boxを作成
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	// 初期サイズ
	TriggerBox->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));

	// Overlap専用設定
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetGenerateOverlapEvents(true);

	// ゲーム中は非表示
	TriggerBox->SetHiddenInGame(true);

	// Overlapイベントをバインド
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASimpleTriggerActor::OnTriggerBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ASimpleTriggerActor::OnTriggerEndOverlap);
}

// =========================
// Overlap開始
// =========================

void ASimpleTriggerActor::OnTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// 自分自身や無効なActorは無視
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	// 1回限定トリガーで、すでに発火済みなら無視
	if (bTriggerOnce && bTriggered)
	{
		return;
	}

	// 発火済みとして記録
	bTriggered = true;

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Log, TEXT("Trigger Activated by %s"), *OtherActor->GetName());
#endif

	// 必要ならトリガーSEを2D再生
	if (TriggerSE)
	{
		UGameplayStatics::PlaySound2D(
			this,
			TriggerSE,
			TriggerSEVolume,
			TriggerSEPitch
		);
	}

	// 実際の処理はBlueprint側へ委譲
	OnTriggered(OtherActor);
}

// =========================
// Overlap終了
// =========================

void ASimpleTriggerActor::OnTriggerEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	// 自分自身や無効なActorは無視
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Log, TEXT("Trigger Released by %s"), *OtherActor->GetName());
#endif

	OnTriggerReleased(OtherActor);
}