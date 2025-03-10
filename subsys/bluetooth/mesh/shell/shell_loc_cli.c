/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <stdlib.h>
#include <bluetooth/mesh/models.h>
#include <zephyr/shell/shell.h>

#include "mesh/net.h"
#include "mesh/access.h"
#include "shell_utils.h"

static struct bt_mesh_model *mod;

static void global_loc_print(const struct shell *shell, int err, struct bt_mesh_loc_global *rsp)
{
	if (!err) {
		shell_print(shell, "Latitude: %f, longitude: %f, altitude: %d", rsp->latitude,
			    rsp->longitude, rsp->altitude);
	}
}

static int cmd_loc_global_get(const struct shell *shell, size_t argc, char *argv[])
{
	if (!mod && !shell_model_first_get(BT_MESH_MODEL_ID_GEN_LOCATION_CLI, &mod)) {
		return -ENODEV;
	}

	struct bt_mesh_loc_cli *cli = mod->user_data;
	struct bt_mesh_loc_global rsp;

	int err = bt_mesh_loc_cli_global_get(cli, NULL, &rsp);

	global_loc_print(shell, err, &rsp);
	return err;
}

static int global_set(const struct shell *shell, size_t argc, char *argv[], bool acked)
{
	double latitude = shell_model_str2dbl(shell, argv[1]);
	double longitude = shell_model_str2dbl(shell, argv[2]);
	int16_t altitude = (int16_t)strtol(argv[3], NULL, 0);

	if (!mod && !shell_model_first_get(BT_MESH_MODEL_ID_GEN_LOCATION_CLI, &mod)) {
		return -ENODEV;
	}

	struct bt_mesh_loc_cli *cli = mod->user_data;
	struct bt_mesh_loc_global set = {
		.latitude = latitude,
		.longitude = longitude,
		.altitude = altitude,
	};

	if (acked) {
		struct bt_mesh_loc_global rsp;
		int err = bt_mesh_loc_cli_global_set(cli, NULL, &set, &rsp);

		global_loc_print(shell, err, &rsp);
		return err;
	} else {
		return bt_mesh_loc_cli_global_set_unack(cli, NULL, &set);
	}
}

static int cmd_loc_global_set(const struct shell *shell, size_t argc, char *argv[])
{
	return global_set(shell, argc, argv, true);
}

static int cmd_loc_global_set_unack(const struct shell *shell, size_t argc, char *argv[])
{
	return global_set(shell, argc, argv, false);
}

static void local_loc_print(const struct shell *shell, int err, struct bt_mesh_loc_local *rsp)
{
	if (!err) {
		shell_print(shell,
			    "North: %d, east: %d, altitude: %d, "
			    "floor_number: %d, is_mobile: %d, "
			    "time_delta: %d, precision_mm: %d",
			    rsp->north, rsp->east, rsp->altitude, rsp->floor_number, rsp->is_mobile,
			    rsp->time_delta, rsp->precision_mm);
	}
}

static int cmd_loc_local_get(const struct shell *shell, size_t argc, char *argv[])
{
	if (!mod && !shell_model_first_get(BT_MESH_MODEL_ID_GEN_LOCATION_CLI, &mod)) {
		return -ENODEV;
	}

	struct bt_mesh_loc_cli *cli = mod->user_data;
	struct bt_mesh_loc_local rsp;

	int err = bt_mesh_loc_cli_local_get(cli, NULL, &rsp);

	local_loc_print(shell, err, &rsp);
	return err;
}

static int local_set(const struct shell *shell, size_t argc, char *argv[], bool acked)
{
	int16_t north = (int16_t)strtol(argv[1], NULL, 0);
	int16_t east = (int16_t)strtol(argv[2], NULL, 0);
	int16_t altitude = (int16_t)strtol(argv[3], NULL, 0);
	int16_t floor = (int16_t)strtol(argv[4], NULL, 0);
	int32_t time_delta = (argc >= 6) ? (int32_t)strtol(argv[5], NULL, 0) : 0;
	uint32_t precision_mm = (argc >= 7) ? (uint32_t)strtol(argv[6], NULL, 0) : 0;
	bool is_mobile = (argc == 8) ? (bool)strtol(argv[7], NULL, 0) : false;

	if (!mod && !shell_model_first_get(BT_MESH_MODEL_ID_GEN_LOCATION_CLI, &mod)) {
		return -ENODEV;
	}

	struct bt_mesh_loc_cli *cli = mod->user_data;
	struct bt_mesh_loc_local set = {
		.north = north,
		.east = east,
		.altitude = altitude,
		.floor_number = floor,
		.is_mobile = is_mobile,
		.time_delta = time_delta,
		.precision_mm = precision_mm,
	};

	if (acked) {
		struct bt_mesh_loc_local rsp;
		int err = bt_mesh_loc_cli_local_set(cli, NULL, &set, &rsp);

		local_loc_print(shell, err, &rsp);
		return err;
	} else {
		return bt_mesh_loc_cli_local_set_unack(cli, NULL, &set);
	}
}

static int cmd_loc_local_set(const struct shell *shell, size_t argc, char *argv[])
{
	return local_set(shell, argc, argv, true);
}

static int cmd_loc_local_set_unack(const struct shell *shell, size_t argc, char *argv[])
{
	return local_set(shell, argc, argv, false);
}

static int cmd_instance_get_all(const struct shell *shell, size_t argc, char *argv[])
{
	return shell_model_instances_get_all(shell, BT_MESH_MODEL_ID_GEN_LOCATION_CLI);
}

static int cmd_instance_set(const struct shell *shell, size_t argc, char *argv[])
{
	uint8_t elem_idx = (uint8_t)strtol(argv[1], NULL, 0);

	return shell_model_instance_set(shell, &mod, BT_MESH_MODEL_ID_GEN_LOCATION_CLI, elem_idx);
}

SHELL_STATIC_SUBCMD_SET_CREATE(instance_cmds,
			       SHELL_CMD_ARG(set, NULL, "<elem_idx> ", cmd_instance_set, 2, 0),
			       SHELL_CMD_ARG(get-all, NULL, NULL, cmd_instance_get_all, 1, 0),
			       SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(
	loc_cmds, SHELL_CMD_ARG(global-get, NULL, NULL, cmd_loc_global_get, 1, 0),
	SHELL_CMD_ARG(global-set, NULL, "<latitude> <longitude> <altitude>", cmd_loc_global_set,
		      4, 0),
	SHELL_CMD_ARG(global-set-unack, NULL, "<latitude> <longitude> <altitude>",
		      cmd_loc_global_set_unack, 4, 0),
	SHELL_CMD_ARG(local-get, NULL, NULL, cmd_loc_local_get, 1, 0),
	SHELL_CMD_ARG(local-set, NULL,
		      "<north> <east> <altitude> <floor> "
		      "[time_delta [precision_mm [is_mobile]]]",
		      cmd_loc_local_set, 5, 3),
	SHELL_CMD_ARG(local-set-unack, NULL,
		      "<north> <east> <altitude> <floor> "
		      "[time_delta [precision_mm [is_mobile]]]",
		      cmd_loc_local_set_unack, 5, 3),
	SHELL_CMD(instance, &instance_cmds, "Instance commands", shell_model_cmds_help),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_ARG_REGISTER(mdl_loc, &loc_cmds, "Location Cli commands", shell_model_cmds_help, 1, 1);
