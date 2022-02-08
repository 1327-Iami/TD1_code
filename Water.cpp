#include "water.h"

void WaterUpdate(MapChip& mapchip, Player* katikoti, Player* gutsugutsu) {

	//���̍X�V�����͊�{�I��water.updateTimer����Ɉ�Ăɍs����悤�ɂ��Ă���
	//�܂��Z����Ì��̏�ԕω��Ɏ��Ԃ�������悤�ɂ������̂�water.updateCount���g���čX�V�܂łɒx���𔭐�������悤�ɂ���

	for (int y = MAP_HEIGHT - 2; 0 <= y; y--) {
		for (int x = MAP_WIDTH - 2; 0 <= x; x--) {

			//�ő̂̎�
			if (mapchip.water.map[y][x] == SOLID) {
				//�O�c�O�c���߂Â�����
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						if ((y == gutsugutsu->player.mapColider.leftBottomY + i && x == gutsugutsu->player.mapColider.leftBottomX + j) ||
							(y == gutsugutsu->player.mapColider.leftTopY + i && x == gutsugutsu->player.mapColider.leftTopX + j) ||
							(y == gutsugutsu->player.mapColider.rightBottomY + i && x == gutsugutsu->player.mapColider.rightBottomX + j) ||
							(y == gutsugutsu->player.mapColider.rightTopY + i && x == gutsugutsu->player.mapColider.rightTopX + j)) {
							//�Z�����n�߂�
							mapchip.water.map[y][x] = MELTING;
							mapchip.water.updateCount[y][x] = 20;
						}
					}
				}
			}

			//�Z�����Ă鎞
			else if (mapchip.water.map[y][x] == MELTING) {
				//��莞�Ԍo������t�̂ɂȂ�
				if (mapchip.water.updateTimer <= 0) {
					mapchip.water.updateCount[y][x]--;
					if (mapchip.water.updateCount[y][x] <= 0) {
						mapchip.water.map[y][x] = LIQUID;
					}
				}

				//�J�`�R�`���߂Â�����Ăьő̂ɂȂ�
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						if ((y == katikoti->player.mapColider.leftBottomY + i && x == katikoti->player.mapColider.leftBottomX + j) ||
							(y == katikoti->player.mapColider.leftTopY + i && x == katikoti->player.mapColider.leftTopX + j) ||
							(y == katikoti->player.mapColider.rightBottomY + i && x == katikoti->player.mapColider.rightBottomX + j) ||
							(y == katikoti->player.mapColider.rightTopY + i && x == katikoti->player.mapColider.rightTopX + j)) {
							mapchip.water.map[y][x] = SOLID;
						}
					}
				}
			}

			//�t�̂̎�
			else if (mapchip.water.map[y][x] == LIQUID) {
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						//�J�`�R�`���߂Â�����ő̂ɂȂ�
						if ((y == katikoti->player.mapColider.leftBottomY + i && x == katikoti->player.mapColider.leftBottomX + j) ||
							(y == katikoti->player.mapColider.leftTopY + i && x == katikoti->player.mapColider.leftTopX + j) ||
							(y == katikoti->player.mapColider.rightBottomY + i && x == katikoti->player.mapColider.rightBottomX + j) ||
							(y == katikoti->player.mapColider.rightTopY + i && x == katikoti->player.mapColider.rightTopX + j)) {
							mapchip.water.map[y][x] = SOLID;
						}

						//�O�c�O�c���߂Â�����C�̂ɂȂ�
						else if ((y == gutsugutsu->player.mapColider.leftBottomY + i && x == gutsugutsu->player.mapColider.leftBottomX + j) ||
							(y == gutsugutsu->player.mapColider.leftTopY + i && x == gutsugutsu->player.mapColider.leftTopX + j) ||
							(y == gutsugutsu->player.mapColider.rightBottomY + i && x == gutsugutsu->player.mapColider.rightBottomX + j) ||
							(y == gutsugutsu->player.mapColider.rightTopY + i && x == gutsugutsu->player.mapColider.rightTopX + j)) {
							mapchip.water.map[y][x] = GAS;
						}
					}
				}
				//updateTimer��0�ȉ��̎�
				if (mapchip.water.updateTimer <= 0) {
					if (mapchip.topography[y + 1][x] == 9) {
						mapchip.water.map[y][x] = 0;
					}
					//�}�b�v�`�b�v�̈���̈ʒu���u���b�N�ł͂Ȃ��Ƃ�
					else if (mapchip.topography[y + 1][x] <= NONE || mapchip.topography[y + 1][x] == THROUGHWATER) {
						if (mapchip.water.map[y + 1][x] != SOLID && mapchip.water.map[y + 1][x] != MELTING) {
							int k = 0;						//�ړ���𒲂ׂ邽�߂Ɏg���ϐ��A���������̖��O���v�����Ȃ�����
							int blockChecker = 0;			//���ׂ��悪�u���b�N�ł��邩�ǂ����o���Ă������߂̕ϐ�
							int isFallLeft = rand() % 2;	//�u���b�N�ɓ������������E�ǂ����ɍs�����A1�Ȃ獶�ɍs��
							//���[�v�����łǂ��Ɉړ��ł���̂��𒲂ׂ�
							while (true) {
								//���ׂ��悪�u���b�N�Ȃ�blockChecker���C���N�������g����i��-1��������
								if (NONE < mapchip.topography[y + 1][x + k] && mapchip.topography[y + 1][x + k] != THROUGHWATER) {
									blockChecker++;
									if (2 <= blockChecker) {
										mapchip.water.isFall[y][x] = 0;
										break;
									}
									k *= (-1);
								}

								else if (mapchip.water.map[y + 1][x + k] == SOLID || mapchip.water.map[y + 1][x + k] == MELTING) {
									blockChecker++;
									if (2 <= blockChecker) {
										mapchip.water.isFall[y][x] = 0;
										break;
									}
									k *= (-1);
								}

								//���ׂ��悪�����Ȃ����Ì����Ă��邩�C�̂Ȃ炻���Ɉړ�����
								else if (mapchip.water.map[y + 1][x + k] <= NONE || mapchip.water.map[y + 1][x + k] == CURDING || mapchip.water.map[y + 1][x + k] == GAS) {
									mapchip.water.map[y + 1][x + k] = LIQUID;
									mapchip.water.map[y][x] = NONE;
									mapchip.water.isFall[y + 1][x + k] = 1;
									break;
								}

								//blockChecker��0�̎�
								if (blockChecker == 0) {
									//���ɍs���Ƃ�
									if (isFallLeft == 1) {
										if (k == 0) {
											k = -1;
										}
										else if (0 < k) {
											k = (k * (-1)) - 1;
										}
										else {
											k = k * (-1);
										}
									}
									//�E�ɍs���Ƃ�
									else {
										if (k == 0) {
											k = 1;
										}
										else if (0 < k) {
											k = k * (-1);
										}
										else {
											k = (k * (-1)) + 1;
										}
									}
								}

								//blockChecker��1�̎�
								//���E�ǂ��炩�̐�ɂ̓u���b�N������̂ŕЕ��������ב����邽�߂ɃC���N�������g�������̓f�N�������g������
								else if (blockChecker == 1) {
									if (0 < k) {
										k++;
									}
									else if (k < 0) {
										k--;
									}
								}
							}
						}
						else {
							mapchip.water.isFall[y][x] = 0;
						}
					}
					else {
						mapchip.water.isFall[y][x] = 0;
					}
				}
			}
		}
	}
	for (int y = 1; y < MAP_HEIGHT; y++) {
		for (int x = 1; x < MAP_WIDTH; x++) {

			//�Ì����Ă鎞
			//��莞�Ԍo������t�̂ɂȂ�
			if (mapchip.water.map[y][x] == CURDING) {
				if (mapchip.water.updateTimer <= 0) {
					mapchip.water.updateCount[y][x]--;
					if (mapchip.water.updateCount[y][x] <= 0) {
						mapchip.water.map[y][x] = LIQUID;
					}
				}

				//�O�c�O�c���߂Â�����ĂыC�̂ɂȂ�
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						if ((y == gutsugutsu->player.mapColider.leftBottomY + i && x == gutsugutsu->player.mapColider.leftBottomX + j) ||
							(y == gutsugutsu->player.mapColider.leftTopY + i && x == gutsugutsu->player.mapColider.leftTopX + j) ||
							(y == gutsugutsu->player.mapColider.rightBottomY + i && x == gutsugutsu->player.mapColider.rightBottomX + j) ||
							(y == gutsugutsu->player.mapColider.rightTopY + i && x == gutsugutsu->player.mapColider.rightTopX + j)) {
							mapchip.water.map[y][x] = GAS;
						}
					}
				}
			}

			//�C�̂̎�
			else if (mapchip.water.map[y][x] == GAS) {
				//�J�`�R�`���߂Â�����Ì����n�߂�
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						if ((y == katikoti->player.mapColider.leftBottomY + i && x == katikoti->player.mapColider.leftBottomX + j) ||
							(y == katikoti->player.mapColider.leftTopY + i && x == katikoti->player.mapColider.leftTopX + j) ||
							(y == katikoti->player.mapColider.rightBottomY + i && x == katikoti->player.mapColider.rightBottomX + j) ||
							(y == katikoti->player.mapColider.rightTopY + i && x == katikoti->player.mapColider.rightTopX + j)) {
							mapchip.water.map[y][x] = CURDING;
							mapchip.water.updateCount[y][x] = 20;
						}
					}
				}
				//updateTimer��0�ȉ��̎�
				if (mapchip.water.updateTimer <= 0) {

					//���̃}�b�v�`�b�v�̈�オ���̎����ł���
					if (mapchip.water.map[y - 1][x] == LIQUID) {
						mapchip.water, mapchip.topography[y][x] <= NONE;
					}

					//�}�b�v�`�b�v�̈��̈ʒu���u���b�N�ł͂Ȃ��Ƃ�
					else if (mapchip.topography[y - 1][x] <= NONE || mapchip.topography[y - 1][x] == THROUGHWATER) {
						if (mapchip.water.map[y - 1][x] != SOLID && mapchip.water.map[y - 1][x] != MELTING) {
							int k = 0;							//�ړ���𒲂ׂ邽�߂Ɏg���ϐ��A���������̖��O���v�����Ȃ�����
							int blockChecker = 0;				//���ׂ��悪�u���b�N�ł��邩�ǂ����o���Ă������߂̕ϐ�
							int isFloatLeft = rand() % 2;		//�u���b�N�ɓ������������E�ǂ����ɍs�����A1�Ȃ獶�ɍs��
							//���[�v�����łǂ��Ɉړ��ł���̂��𒲂ׂ�
							while (true) {
								//���ׂ��悪�u���b�N�Ȃ�blockChecker���C���N�������g����i��-1��������
								if (NONE < mapchip.topography[y - 1][x + k] && mapchip.topography[y - 1][x + k] != THROUGHWATER) {
									blockChecker++;
									if (2 <= blockChecker) {
										break;
									}
									k *= (-1);
								}
								if (mapchip.water.map[y - 1][x] == SOLID || mapchip.water.map[y - 1][x] == MELTING) {
									blockChecker++;
									if (2 <= blockChecker) {
										break;
									}
									k *= (-1);
								}

								//���ׂ��悪�����Ȃ���΂����Ɉړ�����
								else if (mapchip.water.map[y - 1][x + k] <= NONE) {
									mapchip.water.map[y - 1][x + k] = GAS;
									mapchip.water.map[y][x] = 0;
									break;
								}

								//blockChecker��0�̎�
								if (blockChecker == 0) {
									//���ɍs���Ƃ�
									if (isFloatLeft == 1) {
										if (k == 0) {
											k = -1;
										}
										else if (0 <= k) {
											k = k * (-1) - 1;
										}
										else {
											k = k * (-1);
										}
									}
									//���̑�(�E�ɍs���Ƃ�)
									else {
										if (k == 0) {
											k = 1;
										}
										else if (0 < k) {
											k = k * (-1);
										}
										else {
											k = (k * (-1)) + 1;
										}
									}
								}

								//blockChecker��1�̎�
								//���E�ǂ��炩�̐�ɂ̓u���b�N������̂ŕЕ��������ב����邽�߂ɃC���N�������g�������̓f�N�������g������
								else if (blockChecker == 1) {
									if (0 < k) {
										k++;
									}
									else if (k < 0) {
										k--;
									}
								}
							}
						}
					}
					else if (mapchip.topography[y - 1][x] == 9) {
						mapchip.water.map[y][x] = 0;
					}
				}
			}
		}
	}
	if (mapchip.water.updateTimer <= 0) {
		mapchip.water.updateTimer = 5;
	}
	else {
		mapchip.water.updateTimer--;
	}

	mapchip.gasTimer++;
	if (39 < mapchip.gasTimer) {
		mapchip.gasTimer = 0;
	}

	mapchip.iceTimer++;
	if (74 < mapchip.iceTimer) {
		mapchip.iceTimer = 0;
	}

	mapchip.waterTimer++;
	if (32 * 5 - 1 < mapchip.waterTimer) {
		mapchip.waterTimer = 0;
	}

}

void WaterDraw(MapChip mapchip, Scroll scroll, Resource resource) {
	for (int y = MAP_HEIGHT - 1; 0 <= y; y--) {
		for (int x = MAP_WIDTH - 1; 0 <= x; x--) {
			if (mapchip.water.map[y][x] == LIQUID) {
				DrawGraph(x * MAP_SIZE - scroll.x, y * MAP_SIZE - scroll.y, resource.waterCircle[(mapchip.waterTimer / 5) * 2], true);
				DrawGraph(x * MAP_SIZE - scroll.x + 16, y * MAP_SIZE - scroll.y, resource.waterCircle[(mapchip.waterTimer / 5) * 2 + 1], true);
				DrawGraph(x * MAP_SIZE - scroll.x, y * MAP_SIZE - scroll.y + 16, resource.waterCircle[(mapchip.waterTimer / 5) * 2 + 64], true);
				DrawGraph(x * MAP_SIZE - scroll.x + 16, y * MAP_SIZE - scroll.y + 16, resource.waterCircle[(mapchip.waterTimer / 5) * 2 + 64 + 1], true);
				if (mapchip.water.map[y - 1][x] == LIQUID) {
					DrawGraph(x * MAP_SIZE - scroll.x, y * MAP_SIZE - scroll.y, resource.waterBlock[(mapchip.waterTimer / 5) * 2], true);
					DrawGraph(x * MAP_SIZE - scroll.x + 16, y * MAP_SIZE - scroll.y, resource.waterBlock[(mapchip.waterTimer / 5) * 2 + 1], true);
				}

				if (mapchip.water.map[y + 1][x] == LIQUID) {
					DrawGraph(x * MAP_SIZE - scroll.x, y * MAP_SIZE - scroll.y + 16, resource.waterBlock[(mapchip.waterTimer / 5) * 2 + 64], true);
					DrawGraph(x * MAP_SIZE - scroll.x + 16, y * MAP_SIZE - scroll.y + 16, resource.waterBlock[(mapchip.waterTimer / 5) * 2 + 64 + 1], true);
				}

				if (mapchip.water.map[y][x - 1] == LIQUID) {
					DrawGraph(x * MAP_SIZE - scroll.x, y * MAP_SIZE - scroll.y, resource.waterBlock[(mapchip.waterTimer / 5) * 2], true);
					DrawGraph(x * MAP_SIZE - scroll.x, y * MAP_SIZE - scroll.y + 16, resource.waterBlock[(mapchip.waterTimer / 5) * 2 + 64], true);
				}

				if (mapchip.water.map[y][x + 1] == LIQUID) {
					DrawGraph(x * MAP_SIZE - scroll.x + 16, y * MAP_SIZE - scroll.y, resource.waterBlock[(mapchip.waterTimer / 5) * 2 + 1], true);
					DrawGraph(x * MAP_SIZE - scroll.x + 16, y * MAP_SIZE - scroll.y + 16, resource.waterBlock[(mapchip.waterTimer / 5) * 2 + 64 + 1], true);
				}

				if (NONE < mapchip.topography[y + 1][x] && mapchip.topography[y + 1][x] != THROUGHWATER) {
					DrawGraph(x * MAP_SIZE - scroll.x, y * MAP_SIZE - scroll.y + 16, resource.waterBlock[(mapchip.waterTimer / 5) * 2 + 64], true);
					DrawGraph(x * MAP_SIZE - scroll.x + 16, y * MAP_SIZE - scroll.y + 16, resource.waterBlock[(mapchip.waterTimer / 5) * 2 + 64 + 1], true);
				}
				if (mapchip.water.map[y + 1][x] == SOLID || mapchip.water.map[y + 1][x] == MELTING) {
					DrawGraph(x * MAP_SIZE - scroll.x, y * MAP_SIZE - scroll.y + 16, resource.waterBlock[(mapchip.waterTimer / 5) * 2 + 64], true);
					DrawGraph(x * MAP_SIZE - scroll.x + 16, y * MAP_SIZE - scroll.y + 16, resource.waterBlock[(mapchip.waterTimer / 5) * 2 + 64 + 1], true);
				}
			}

			else if (mapchip.water.map[y][x] == SOLID) {
				DrawGraph(x * MAP_SIZE - scroll.x, y * MAP_SIZE - scroll.y, resource.ice[mapchip.iceTimer / 5], true);
			}

			else if (mapchip.water.map[y][x] == MELTING) {
				DrawGraph(x * MAP_SIZE - scroll.x, y * MAP_SIZE - scroll.y, resource.melting, true);
			}

			else if (mapchip.water.map[y][x] == CURDING) {
				DrawGraph(x * MAP_SIZE - scroll.x, y * MAP_SIZE - scroll.y, resource.curding, true);
			}

			else if (mapchip.water.map[y][x] == GAS) {
				DrawRotaGraph3(x * MAP_SIZE - scroll.x + 16, y * MAP_SIZE - scroll.y + 16, 16, 16, 3, 3, 0, resource.gas[mapchip.gasTimer / 5], true);
			}
		}
	}
}