#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ObjectIcon.generated.h"

/**
 * ミニマップ上に表示するオブジェクト用アイコン
 *
 * 主な役割：
 * ・対象オブジェクトのワールド位置を保持する
 * ・ミニマップ基準位置と回転から2D座標を計算する
 * ・必要に応じてアイコンのTransformを更新する
 */
UCLASS()
class SWING_API AObjectIcon : public AActor
{
	GENERATED_BODY()

public:
	AObjectIcon();

protected:
	/** ゲーム開始時に呼ばれる */
	virtual void BeginPlay() override;

public:
	/** 毎フレーム更新 */
	virtual void Tick(float DeltaTime) override;

	// =========================
	// ミニマップ設定
	// =========================

	/** ミニマップの中心位置（プレイヤー位置など） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	FVector MinimapCenter;

	/** ミニマップの回転（Yaw） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	float MinimapYaw = 0.0f;

	/** ミニマップの表示スケール */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	float MinimapScale = 1.0f;

	/** 表示対象オブジェクトのワールド位置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	FVector PlanetWorldPosition;

	// =========================
	// 表示用コンポーネント
	// =========================

	/** アイコン表示用メッシュ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap")
	UStaticMeshComponent* IconMesh;

	// =========================
	// ミニマップ計算
	// =========================

	/** ミニマップ上の2D座標を取得する */
	UFUNCTION(BlueprintCallable, Category = "Minimap")
	FVector2D GetMinimapPosition() const;

	/** ミニマップ上のYaw回転を取得する */
	UFUNCTION(BlueprintCallable, Category = "Minimap")
	float GetMinimapYaw() const;

private:
	/** アイコンの位置や回転を更新する */
	void UpdateIconTransform();
};