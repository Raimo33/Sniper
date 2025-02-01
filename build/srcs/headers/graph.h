/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:19:17 by craimond          #+#    #+#             */
/*   Updated: 2025/02/01 11:55:15 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPH_H
# define GRAPH_H

//all available pairs, with their fees and minimum order size and precision
//Represent currencies as nodes and exchange rates as weighted edges.
//Precompute All Triangular Cycles 
//SIMD-Based Cycle Evaluation

# include "fixed_point.h"

# define TICKER_SIZE 5

typedef struct
{
  const char name[TICKER_SIZE];
  const uint8_t precision;  
} currency_t;

typedef struct
{
  const currency_t base;
  const currency_t quote;
  const fixed_point_t fee;
  //https://developers.binance.com/docs/binance-spot-api-docs/filters
  const fixed_point_t min_price;
  const fixed_point_t max_price;
  const fixed_point_t tick_size;
  const fixed_point_t min_qty;
  const fixed_point_t max_qty;
  const fixed_point_t step_size;
  fixed_point_t rate;
  //TODO weigh
} trading_pair_t;

//TODO directed graph, weighted edges
typedef struct
{
  const currency_t *restrict currencies;
  const trading_pair_t *restrict pairs;
  const uint16_t currency_count;
  const uint16_t pair_count;
} graph_t;

#endif