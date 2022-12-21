// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Variant.h"

#include "RWSUtilsDataModel.generated.h"

/** Target platform name */
UENUM(BlueprintType)
enum class ERWSPublishingPlatform : uint8
{
	playstation_network UMETA(DisplayName = "PlaystationNetwork"),
	xbox_live UMETA(DisplayName = "XboxLive"),
	rws UMETA(DisplayName = "RWS"),
	pc_standalone UMETA(DisplayName = "PcStandalone"),
	nintendo_shop UMETA(DisplayName = "NintendoShop"),
	google_play UMETA(DisplayName = "GooglePlay"),
	app_store_ios UMETA(DisplayName = "AppStoreIos"),
	android_standalone UMETA(DisplayName = "AndroidStandalone"),
	ios_standalone UMETA(DisplayName = "IosStandalone"),
	android_other UMETA(DisplayName = "AndroidOther"),
	ios_other UMETA(DisplayName = "IosOther"),
	pc_other UMETA(DisplayName = "PcOther")
};

USTRUCT(BlueprintType)
struct RWSUTILS_API FRWSItemGroup
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item Group")
	int32 id;

	UPROPERTY(BlueprintReadOnly, Category = "Item Group")
	FString external_id;

	UPROPERTY(BlueprintReadOnly, Category = "Item Group")
	FString name;

	UPROPERTY(BlueprintReadOnly, Category = "Item Group")
	FString description;

	UPROPERTY(BlueprintReadOnly, Category = "Item Group")
	FString image_url;

	UPROPERTY(BlueprintReadOnly, Category = "Item Group")
	int32 level;

	UPROPERTY(BlueprintReadOnly, Category = "Item Group")
	int32 order;

	UPROPERTY(BlueprintReadOnly, Category = "Item Group")
	FString parent_external_id;

public:
	FRWSItemGroup()
		: id(-1)
		, level(0)
		, order(1){};
};

USTRUCT(BlueprintType)
struct RWSUTILS_API FRWSItemAttributeValue
{
	GENERATED_BODY()

	/** Unique value ID for an attribute. The external_id may only contain lowercase Latin alphanumeric characters, dashes, and underscores. */
	UPROPERTY(BlueprintReadOnly, Category = "Item Attribute Value")
	FString external_id;

	/* Value of attribute. */
	UPROPERTY(BlueprintReadOnly, Category = "Item Attribute Value")
	FString value;
};

/** Attribute and their values corresponding to the item. Can be used for catalog filtering. */
USTRUCT(BlueprintType)
struct RWSUTILS_API FRWSItemAttribute
{
	GENERATED_BODY()

	/** Unique attribute ID. The `external_id` may only contain lowercase Latin alphanumeric characters, dashes, and underscores. */
	UPROPERTY(BlueprintReadOnly, Category = "Item Attribute")
	FString external_id;

	/* Name off attribute. */
	UPROPERTY(BlueprintReadOnly, Category = "Item Attribute")
	FString name;

	UPROPERTY(BlueprintReadOnly, Category = "Item Attribute")
	TArray<FRWSItemAttributeValue> values;

public:
	FRWSItemAttribute(){}
};

USTRUCT(BlueprintType)
struct RWSUTILS_API FRWSPrice
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item Price")
	FString amount;

	UPROPERTY(BlueprintReadOnly, Category = "Item Price")
	FString amount_without_discount;

	UPROPERTY(BlueprintReadOnly, Category = "Item Price")
	FString currency;

public:
	FRWSPrice(){}
};

USTRUCT(BlueprintType)
struct RWSUTILS_API FRWSConsumable
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item Consumable")
	int32 usages_count;

public:
	FRWSConsumable()
		: usages_count(0)
	{
	}
};

USTRUCT(BlueprintType)
struct RWSUTILS_API FRWSExpirationPeriod
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item Expiration Period")
	int32 value;

	UPROPERTY(BlueprintReadOnly, Category = "Item Expiration Period")
	FString type;

public:
	FRWSExpirationPeriod()
		: value(0){}
};

USTRUCT(BlueprintType)
struct RWSUTILS_API FRWSItemOptions
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item Options")
	FRWSConsumable consumable;

	UPROPERTY(BlueprintReadOnly, Category = "Item Options")
	FRWSExpirationPeriod expiration_period;
};

USTRUCT(BlueprintType)
struct RWSUTILS_API FRWSVirtualCurrencyCalculatedPrice
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Virtual Currency Calculated Price")
	FString amount;

	UPROPERTY(BlueprintReadOnly, Category = "Virtual Currency Calculated Price")
	FString amount_without_discount;
};

USTRUCT(BlueprintType)
struct RWSUTILS_API FRWSVirtualCurrencyPrice
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item Virtual Currency Price")
	FString sku;

	UPROPERTY(BlueprintReadOnly, Category = "Item Virtual Currency Price")
	bool is_default;

	UPROPERTY(BlueprintReadOnly, Category = "Item Virtual Currency Price")
	int amount;

	UPROPERTY(BlueprintReadOnly, Category = "Item Virtual Currency Price")
	int amount_without_discount;

	UPROPERTY(BlueprintReadOnly, Category = "Item Virtual Currency Price")
	FString image_url;

	UPROPERTY(BlueprintReadOnly, Category = "Item Virtual Currency Price")
	FString name;

	UPROPERTY(BlueprintReadOnly, Category = "Item Virtual Currency Price")
	FString description;

	UPROPERTY(BlueprintReadOnly, Category = "Item Virtual Currency Price")
	FString type;

	UPROPERTY(BlueprintReadOnly, Category = "Item Virtual Currency Price")
	FRWSVirtualCurrencyCalculatedPrice calculated_price;

public:
	FRWSVirtualCurrencyPrice()
		: is_default(false)
		, amount(0)
		, amount_without_discount(0){}
};

USTRUCT(BlueprintType)
struct RWSUTILS_API FRWSUnitItem
{
public:
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Unit Item")
	FString sku;

	UPROPERTY(BlueprintReadOnly, Category = "Unit Item")
	FString name;

	UPROPERTY(BlueprintReadOnly, Category = "Unit Item")
	FString type;

	UPROPERTY(BlueprintReadOnly, Category = "Unit Item")
	FString drm_name;

	UPROPERTY(BlueprintReadOnly, Category = "Unit Item")
	FString drm_sku;
};

USTRUCT(BlueprintType)
struct RWSUTILS_API FRWSRewardItem
{
public:
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Reward Item")
	FString sku;

	UPROPERTY(BlueprintReadOnly, Category = "Reward Item")
	FString name;

	UPROPERTY(BlueprintReadOnly, Category = "Reward Item")
	FString type;

	UPROPERTY(BlueprintReadOnly, Category = "Reward Item")
	FString virtual_item_type;

	UPROPERTY(BlueprintReadOnly, Category = "Reward Item")
	FString description;

	UPROPERTY(BlueprintReadOnly, Category = "Reward Item")
	FString image_url;

	UPROPERTY(BlueprintReadOnly, Category = "Reward Item")
	TArray<FRWSUnitItem> unit_items;
};

USTRUCT(BlueprintType)
struct RWSUTILS_API FRWSBonusItem
{
public:
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Bonus Item")
	FRWSRewardItem item;

	UPROPERTY(BlueprintReadOnly, Category = "Bonus Item")
	int32 quantity;

public:
	FRWSBonusItem()
		: quantity(0){};
};

USTRUCT(BlueprintType)
struct FRWSJsonVariant
{
	GENERATED_BODY()

	FRWSJsonVariant(){};
	FRWSJsonVariant(const FVariant Variant)
		: Variant(Variant){};

	FVariant Variant;
};

USTRUCT(BlueprintType)
struct FRWSParameters
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "RWS Parameters")
	TMap<FString, FRWSJsonVariant> Parameters;
};

/* Usual version EVariantTypes isn't using UENUM(). It causes the problem when calling GetEnumValueAsString with "EVariantTypes" as the first argument.
*/
UENUM()
enum class ERWSVariantTypes : int8
{
	Empty = 0,
	Ansichar = 1,
	Bool = 2,
	Box = 3,
	BoxSphereBounds = 4,
	ByteArray = 5,
	Color = 6,
	DateTime = 7,
	Double = 8,
	Enum = 9,
	Float = 10,
	Guid = 11,
	Int8 = 12,
	Int16 = 13,
	Int32 = 14,
	Int64 = 15,
	IntRect = 16,
	LinearColor = 17,
	Matrix = 18,
	Name = 19,
	Plane = 20,
	Quat = 21,
	RandomStream = 22,
	Rotator = 23,
	String = 24,
	Widechar = 25,
	Timespan = 26,
	Transform = 27,
	TwoVectors = 28,
	UInt8 = 29,
	UInt16 = 30,
	UInt32 = 31,
	UInt64 = 32,
	Vector = 33,
	Vector2d = 34,
	Vector4 = 35,
	IntPoint = 36,
	IntVector = 37,
	NetworkGUID = 38,
	Custom = 0x40
};