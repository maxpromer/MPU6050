Blockly.Blocks['mpu6050_acc'] = {
	init: function() {
		this.jsonInit({
			"type": "mpu6050_acc",
			"message0": Blockly.Msg.MPU6050_ACC_MESSAGE,
			"args0": [
				{
				"type": "field_dropdown",
				"name": "axis",
				"options": [
					[
					"x",
					"1"
					],
					[
					"y",
					"2"
					],
					[
					"z",
					"3"
					],
					[
					"strength",
					"4"
					]
				]
				}
			],
			"output": "Number",
			"colour": 285,
			"tooltip": Blockly.Msg.MPU6050_ACC_TOOLTIP,
			"helpUrl": "https://www.gravitechthai.com/"
		});
	}
};

Blockly.Blocks['mpu6050_gyro'] = {
	init: function() {
		this.jsonInit({
			"type": "mpu6050_gyro",
			"message0": Blockly.Msg.MPU6050_GYRO_MESSAGE,
			"args0": [
				{
				"type": "field_dropdown",
				"name": "axis",
				"options": [
					[
					"x",
					"1"
					],
					[
					"y",
					"2"
					],
					[
					"z",
					"3"
					],
					[
					"strength",
					"4"
					]
				]
				}
			],
			"output": "Number",
			"colour": 285,
			"tooltip": Blockly.Msg.MPU6050_GYRO_TOOLTIP,
			"helpUrl": "https://www.gravitechthai.com/"
		});
	}
};

Blockly.Blocks['mpu6050_temp'] = {
	init: function() {
		this.jsonInit({
			"type": "mpu6050_temp",
			"message0": Blockly.Msg.MPU6050_TEMP_MESSAGE,
			"output": "Number",
			"colour": 285,
			"tooltip": Blockly.Msg.MPU6050_TEMP_TOOLTIP,
			"helpUrl": "https://www.gravitechthai.com/"
		});
	}
};

Blockly.Blocks['mpu6050_acc_set_range'] = {
	init: function() {
		this.jsonInit({
			"type": "mpu6050_acc_set_range",
			"message0": Blockly.Msg.MPU6050_ACC_SET_RANGE_MESSAGE,
			"args0": [
				{
				"type": "field_dropdown",
				"name": "range",
				"options": [
					[
					"2g",
					"2"
					],
					[
					"4g",
					"4"
					],
					[
					"8g",
					"8"
					],
					[
					"16g",
					"16"
					]
				]
				}
			],
			"previousStatement": null,
			"nextStatement": null,
			"colour": 285,
			"tooltip": Blockly.Msg.MPU6050_ACC_SET_RANGE_TOOLTIP,
			"helpUrl": "https://www.gravitechthai.com/"
		});
	}
};


Blockly.Blocks['mpu6050_gyro_set_range'] = {
	init: function() {
		this.jsonInit({
			"type": "mpu6050_gyro_set_range",
			"message0": Blockly.Msg.MPU6050_GYRO_SET_RANGE_MESSAGE,
			"args0": [
				{
				"type": "field_dropdown",
				"name": "range",
				"options": [
					[
					"250°/s",
					"250"
					],
					[
					"500°/s",
					"500"
					],
					[
					"1000°/s",
					"1000"
					],
					[
					"2000°/s",
					"2000"
					]
				]
				}
			],
			"previousStatement": null,
			"nextStatement": null,
			"colour": 285,
			"tooltip": Blockly.Msg.MPU6050_GYRO_SET_RANGE_TOOLTIP,
			"helpUrl": "https://www.gravitechthai.com/"
		});
	}
}
