/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:09:17 by craimond          #+#    #+#             */
/*   Updated: 2025/02/03 13:36:15 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/graph.h"

COLD static uint16_t add_currency(graph_t *restrict graph, const currency_t *restrict currency);

void init_graph(graph_t *restrict graph)
{
  graph->currencies = malloc(N_EXPECTED_CURRENCIES * sizeof(currency_t));
  graph->pairs = malloc(N_EXPECTED_PAIRS * sizeof(trading_pair_t));
  graph->adjacency_lists = malloc(N_EXPECTED_CURRENCIES * sizeof(adjacency_list_t));
  graph->n_currencies = 0;
  graph->n_pairs = 0;
  graph->n_allocated_currencies = N_EXPECTED_CURRENCIES;
  graph->n_allocated_pairs = N_EXPECTED_PAIRS;
}

void add_pair(graph_t *restrict graph, trading_pair_t *restrict pair)
{
  const uint16_t base_idx = add_currency(graph, &pair->base_currency); //TODO adds currency if it doesn't exist, and allocs the adjency list of that currency
  const uint16_t quote_idx = add_currency(graph, &pair->quote_currency);

  fast_assert(base_idx != quote_idx, STR_AND_LEN("Base and quote currencies must be different"));

  if (graph->n_pairs == graph->n_allocated_pairs)
    graph->pairs = realloc(graph->pairs, (++graph->n_allocated_pairs) * sizeof(trading_pair_t));
  memcpy(&graph->pairs[graph->n_pairs++], pair, sizeof(trading_pair_t));

  const fixed_point_t fee_weight = fast_log2(fast_sub(FIXED_ONE, pair->fee));

  edge_t edge = {
    .target_idx = quote_idx,
    .fee_weight = fee_weight,
    .weight = FIXED_NEG_INF
  };

  adjacency_list_t *base_adj = &graph->adjacency_lists[base_idx];
  base_adj->edges = realloc(base_adj->edges, (base_adj->n_edges + 1) * sizeof(edge_t));
  memcpy(&base_adj->edges[base_adj->n_edges++], &edge, sizeof(edge_t));
}

COLD static uint16_t add_currency(graph_t *restrict graph, const currency_t *restrict currency)
{
  for (uint16_t i = 0; i < graph->n_currencies; i++)
  {
    if (memcmp(&graph->currencies[i], currency, sizeof(currency_t)) == 0)
      return i;
  }

  if (graph->n_currencies == graph->n_allocated_currencies)
  {
    graph->currencies = realloc(graph->currencies, (graph->n_allocated_currencies + 1) * sizeof(currency_t));
    graph->adjacency_lists = realloc(graph->adjacency_lists, (graph->n_allocated_currencies + 1) * sizeof(adjacency_list_t));
    graph->n_allocated_currencies++;
  }
  memcpy(&graph->currencies[graph->n_currencies], currency, sizeof(currency_t));
  graph->adjacency_lists[graph->n_currencies] = (adjacency_list_t){0};

  return graph->n_currencies++;
}

void free_graph(graph_t *restrict graph)
{
  //TODO
  (void)graph;
}