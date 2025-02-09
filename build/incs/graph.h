/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:19:17 by craimond          #+#    #+#             */
/*   Updated: 2025/02/09 16:07:45 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPH_H
# define GRAPH_H

# include <stdint.h>
# include <stdlib.h>

# include "primitives/alloc_primitives.h"
# include "fixed_point.h"
# include "extensions.h"
# include "errors.h"

# define TICKER_SIZE 5
# define N_EXPECTED_PAIRS 1300
# define N_EXPECTED_CURRENCIES 400

typedef struct
{
  const char name[TICKER_SIZE];
  const uint8_t precision;
} currency_t;

//https://developers.binance.com/docs/binance-spot-api-docs/filters
typedef struct
{
  const currency_t base_currency;
  const currency_t quote_currency;
  const fixed_point_t fee;
  const fixed_point_t min_price;
  const fixed_point_t max_price;
  const fixed_point_t tick_size;
  const fixed_point_t min_qty;
  const fixed_point_t max_qty;
  const fixed_point_t step_size;
  fixed_point_t rate;
} trading_pair_t;

typedef struct
{
  const uint16_t target_idx;
  const fixed_point_t fee_weight;
  fixed_point_t weight;
} edge_t;

typedef struct
{
  uint16_t n_edges;
  edge_t *edges;
} adjacency_list_t;

typedef struct
{
  currency_t *currencies;
  adjacency_list_t *adjacency_lists;
  trading_pair_t *pairs;
  uint16_t n_currencies;
  uint16_t n_pairs;
  uint16_t n_allocated_currencies;
  uint16_t n_allocated_pairs;
} graph_t;

//TODO higly optimized simd support
COLD void init_graph(graph_t *restrict graph);
COLD void free_graph(graph_t *restrict graph);


#endif