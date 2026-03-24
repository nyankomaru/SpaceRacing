#include "ObjectIcon.h"

// =========================
// コンストラクタ
// =========================

AObjectIcon::AObjectIcon()
{
	// ミニマップ上の表示更新を毎フレーム行う
	PrimaryActorTick.bCanEverTick = true;

	// 表示用メッシュを作成
	IconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IconMesh"));
	RootComponent = IconMesh;

	// アイコンサイズは固定
	IconMesh->SetWorldScale3D(FVector(0.5f));
}

// =========================
// 開始時処理
// =========================

void AObjectIcon::BeginPlay()
{
	Super::BeginPlay();
}

// =========================
// 毎フレーム更新
// =========================

void AObjectIcon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateIconTransform();
}

// =========================
// ミニマップ座標計算
// =========================

FVector2D AObjectIcon::GetMinimapPosition() const
{
	// XY平面上の位置だけを使う
	const FVector2D Center2D(MinimapCenter.X, MinimapCenter.Y);
	const FVector2D Planet2D(PlanetWorldPosition.X, PlanetWorldPosition.Y);

	// ミニマップ中心から見た相対位置をスケール反映込みで計算
	const FVector2D Relative = (Planet2D - Center2D) * MinimapScale;

	// Yaw回転を適用してミニマップ座標へ変換
	const float Rad = FMath::DegreesToRadians(MinimapYaw);
	const float Cos = FMath::Cos(Rad);
	const float Sin = FMath::Sin(Rad);

	return FVector2D(
		Relative.X * Cos - Relative.Y * Sin,
		Relative.X * Sin + Relative.Y * Cos
	);
}

float AObjectIcon::GetMinimapYaw() const
{
	return MinimapYaw;
}

// =========================
// アイコン表示更新
// =========================

void AObjectIcon::UpdateIconTransform()
{
	// ミニマップ上の2D座標を3D空間上の位置に変換
	const FVector2D MiniPos = GetMinimapPosition();
	const FVector NewLocation(MiniPos.X, MiniPos.Y, PlanetWorldPosition.Z);

	SetActorLocation(NewLocation);

	// アイコンの向きはミニマップのYawに合わせる
	const FRotator NewRotation(0.0f, GetMinimapYaw(), 0.0f);
	SetActorRotation(NewRotation);

	// サイズは常に一定にする
	IconMesh->SetWorldScale3D(FVector(0.5f));
}