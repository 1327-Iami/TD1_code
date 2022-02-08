#include "water.h"

void WaterUpdate(MapChip& mapchip, Player* katikoti, Player* gutsugutsu) {

	//水の更新処理は基本的にwater.updateTimerを基準に一斉に行われるようにしている
	//また融解や凝結の状態変化に時間をかけるようにしたものはwater.updateCountを使って更新までに遅延を発生させるようにした

	for (int y = MAP_HEIGHT - 2; 0 <= y; y--) {
		for (int x = MAP_WIDTH - 2; 0 <= x; x--) {

			//固体の時
			if (mapchip.water.map[y][x] == SOLID) {
				//グツグツが近づいたら
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						if ((y == gutsugutsu->player.mapColider.leftBottomY + i && x == gutsugutsu->player.mapColider.leftBottomX + j) ||
							(y == gutsugutsu->player.mapColider.leftTopY + i && x == gutsugutsu->player.mapColider.leftTopX + j) ||
							(y == gutsugutsu->player.mapColider.rightBottomY + i && x == gutsugutsu->player.mapColider.rightBottomX + j) ||
							(y == gutsugutsu->player.mapColider.rightTopY + i && x == gutsugutsu->player.mapColider.rightTopX + j)) {
							//融解を始める
							mapchip.water.map[y][x] = MELTING;
							mapchip.water.updateCount[y][x] = 20;
						}
					}
				}
			}

			//融解してる時
			else if (mapchip.water.map[y][x] == MELTING) {
				//一定時間経ったら液体になる
				if (mapchip.water.updateTimer <= 0) {
					mapchip.water.updateCount[y][x]--;
					if (mapchip.water.updateCount[y][x] <= 0) {
						mapchip.water.map[y][x] = LIQUID;
					}
				}

				//カチコチが近づいたら再び固体になる
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

			//液体の時
			else if (mapchip.water.map[y][x] == LIQUID) {
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						//カチコチが近づいたら固体になる
						if ((y == katikoti->player.mapColider.leftBottomY + i && x == katikoti->player.mapColider.leftBottomX + j) ||
							(y == katikoti->player.mapColider.leftTopY + i && x == katikoti->player.mapColider.leftTopX + j) ||
							(y == katikoti->player.mapColider.rightBottomY + i && x == katikoti->player.mapColider.rightBottomX + j) ||
							(y == katikoti->player.mapColider.rightTopY + i && x == katikoti->player.mapColider.rightTopX + j)) {
							mapchip.water.map[y][x] = SOLID;
						}

						//グツグツが近づいたら気体になる
						else if ((y == gutsugutsu->player.mapColider.leftBottomY + i && x == gutsugutsu->player.mapColider.leftBottomX + j) ||
							(y == gutsugutsu->player.mapColider.leftTopY + i && x == gutsugutsu->player.mapColider.leftTopX + j) ||
							(y == gutsugutsu->player.mapColider.rightBottomY + i && x == gutsugutsu->player.mapColider.rightBottomX + j) ||
							(y == gutsugutsu->player.mapColider.rightTopY + i && x == gutsugutsu->player.mapColider.rightTopX + j)) {
							mapchip.water.map[y][x] = GAS;
						}
					}
				}
				//updateTimerが0以下の時
				if (mapchip.water.updateTimer <= 0) {
					if (mapchip.topography[y + 1][x] == 9) {
						mapchip.water.map[y][x] = 0;
					}
					//マップチップの一つ下の位置がブロックではないとき
					else if (mapchip.topography[y + 1][x] <= NONE || mapchip.topography[y + 1][x] == THROUGHWATER) {
						if (mapchip.water.map[y + 1][x] != SOLID && mapchip.water.map[y + 1][x] != MELTING) {
							int k = 0;						//移動先を調べるために使う変数、いい感じの名前が思いつかなかった
							int blockChecker = 0;			//調べた先がブロックであるかどうか覚えておくための変数
							int isFallLeft = rand() % 2;	//ブロックに当たった時左右どっちに行くか、1なら左に行く
							//ループ処理でどこに移動できるのかを調べる
							while (true) {
								//調べた先がブロックならblockCheckerをインクリメントしてiに-1をかける
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

								//調べた先が何もないか凝結しているか気体ならそこに移動する
								else if (mapchip.water.map[y + 1][x + k] <= NONE || mapchip.water.map[y + 1][x + k] == CURDING || mapchip.water.map[y + 1][x + k] == GAS) {
									mapchip.water.map[y + 1][x + k] = LIQUID;
									mapchip.water.map[y][x] = NONE;
									mapchip.water.isFall[y + 1][x + k] = 1;
									break;
								}

								//blockCheckerが0の時
								if (blockChecker == 0) {
									//左に行くとき
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
									//右に行くとき
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

								//blockCheckerが1の時
								//左右どちらかの先にはブロックがあるので片方だけ調べ続けるためにインクリメントもしくはデクリメントをする
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

			//凝結してる時
			//一定時間経ったら液体になる
			if (mapchip.water.map[y][x] == CURDING) {
				if (mapchip.water.updateTimer <= 0) {
					mapchip.water.updateCount[y][x]--;
					if (mapchip.water.updateCount[y][x] <= 0) {
						mapchip.water.map[y][x] = LIQUID;
					}
				}

				//グツグツが近づいたら再び気体になる
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

			//気体の時
			else if (mapchip.water.map[y][x] == GAS) {
				//カチコチが近づいたら凝結を始める
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
				//updateTimerが0以下の時
				if (mapchip.water.updateTimer <= 0) {

					//水のマップチップの一つ上が水の時消滅する
					if (mapchip.water.map[y - 1][x] == LIQUID) {
						mapchip.water, mapchip.topography[y][x] <= NONE;
					}

					//マップチップの一つ上の位置がブロックではないとき
					else if (mapchip.topography[y - 1][x] <= NONE || mapchip.topography[y - 1][x] == THROUGHWATER) {
						if (mapchip.water.map[y - 1][x] != SOLID && mapchip.water.map[y - 1][x] != MELTING) {
							int k = 0;							//移動先を調べるために使う変数、いい感じの名前が思いつかなかった
							int blockChecker = 0;				//調べた先がブロックであるかどうか覚えておくための変数
							int isFloatLeft = rand() % 2;		//ブロックに当たった時左右どっちに行くか、1なら左に行く
							//ループ処理でどこに移動できるのかを調べる
							while (true) {
								//調べた先がブロックならblockCheckerをインクリメントしてiに-1をかける
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

								//調べた先が何もなければそこに移動する
								else if (mapchip.water.map[y - 1][x + k] <= NONE) {
									mapchip.water.map[y - 1][x + k] = GAS;
									mapchip.water.map[y][x] = 0;
									break;
								}

								//blockCheckerが0の時
								if (blockChecker == 0) {
									//左に行くとき
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
									//その他(右に行くとき)
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

								//blockCheckerが1の時
								//左右どちらかの先にはブロックがあるので片方だけ調べ続けるためにインクリメントもしくはデクリメントをする
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