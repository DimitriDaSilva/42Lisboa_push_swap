/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-silv <dda-silv@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 09:55:56 by dda-silv          #+#    #+#             */
/*   Updated: 2021/04/10 09:22:21 by dda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "utils.h"
# include "instructions.h"

void	get_stack(char *stack[], int len, t_list **stack_a);
void	sort_stack(t_list **stack_a, t_list **stack_b);

#endif
