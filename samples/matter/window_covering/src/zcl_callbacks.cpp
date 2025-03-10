/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file
 *   This file implements the handler for data model messages.
 */

#include "window_covering.h"
#include <app_config.h>

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/callback.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/CommandHandler.h>
#include <app/ConcreteAttributePath.h>
#include <app/ConcreteCommandPath.h>
#include <app/clusters/window-covering-server/window-covering-server.h>
#include <app/util/af.h>

using namespace ::chip;
using namespace ::chip::app::Clusters::WindowCovering;

void MatterPostAttributeChangeCallback(const app::ConcreteAttributePath &attributePath, uint8_t mask, uint8_t type,
				       uint16_t size, uint8_t *value)
{
	switch (attributePath.mClusterId) {
	case app::Clusters::Identify::Id:
		ChipLogProgress(Zcl, "Identify cluster ID: " ChipLogFormatMEI " Type: %u Value: %u, length: %u",
				ChipLogValueMEI(attributePath.mAttributeId), type, *value, size);
		break;
	case app::Clusters::WindowCovering::Id:
		ChipLogProgress(Zcl, "Window covering cluster ID: " ChipLogFormatMEI " Type: %u Value: %u, length: %u",
				ChipLogValueMEI(attributePath.mAttributeId), type, *value, size);
		break;
	default:
		break;
	}
}

/* Forwards all attributes changes */
void MatterWindowCoveringClusterServerAttributeChangedCallback(const app::ConcreteAttributePath &attributePath)
{
	if (attributePath.mEndpointId == WindowCovering::Endpoint()) {
		switch (attributePath.mAttributeId) {
		case Attributes::TargetPositionLiftPercent100ths::Id:
			WindowCovering::Instance().StartMove(WindowCovering::MoveType::LIFT);
			break;
		case Attributes::TargetPositionTiltPercent100ths::Id:
			WindowCovering::Instance().StartMove(WindowCovering::MoveType::TILT);
			break;
		case Attributes::CurrentPositionLiftPercent100ths::Id:
			WindowCovering::Instance().PositionLEDUpdate(WindowCovering::MoveType::LIFT);
			break;
		case Attributes::CurrentPositionTiltPercent100ths::Id:
			WindowCovering::Instance().PositionLEDUpdate(WindowCovering::MoveType::TILT);
			break;
		};
	}
}
